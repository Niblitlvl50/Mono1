
#include "ParticleSystem.h"
#include "Rendering/RenderBuffer/IRenderBuffer.h"
#include "Util/Hash.h"
#include "Util/Algorithm.h"
#include "Util/Random.h"

#include <cassert>

using namespace mono;

namespace
{
    void Swap(ParticlePoolComponent& pool_component, size_t first, size_t second)
    {
        std::swap(pool_component.position[first],           pool_component.position[second]);
        std::swap(pool_component.velocity[first],           pool_component.velocity[second]);
        std::swap(pool_component.rotation[first],           pool_component.rotation[second]);
        std::swap(pool_component.angular_velocity[first],   pool_component.angular_velocity[second]);
        std::swap(pool_component.color[first],              pool_component.color[second]);
        std::swap(pool_component.start_color[first],        pool_component.start_color[second]);
        std::swap(pool_component.end_color[first],          pool_component.end_color[second]);
        std::swap(pool_component.start_life[first],         pool_component.start_life[second]);
        std::swap(pool_component.life[first],               pool_component.life[second]);
    }

    void WakeParticle(ParticlePoolComponent& pool_component, size_t index)
    {
        if(pool_component.count_alive < pool_component.pool_size)
        {
            Swap(pool_component, index, pool_component.count_alive);
            ++pool_component.count_alive;
        }
    }

    bool IsDone(const ParticleEmitterComponent& emitter_component)
    {
        if(emitter_component.type == mono::EmitterType::BURST || emitter_component.type == mono::EmitterType::BURST_REMOVE_ON_FINISH)
            return emitter_component.burst_emitted;

        return (emitter_component.duration > 0.0f && emitter_component.elapsed_time > emitter_component.duration);
    }
}

void mono::DefaultGenerator(const math::Vector& position, struct ParticlePoolComponent& pool, size_t index)
{
    const float x = mono::Random(-8.0f, 8.0f);
    const float y = mono::Random(-8.0f, 8.0f);
    const float life = mono::Random(0.0f, 500.0f);

    pool.position[index] = position;
    pool.velocity[index] = math::Vector(x, y);
    pool.rotation[index] = 0.0f;
    pool.angular_velocity[index] = 0.0f;
    //pool.m_color[index];
    pool.start_color[index] = mono::Color::RGBA(1.0f, 0.0f, 0.0f, 1.0f);
    pool.end_color[index] = mono::Color::RGBA(0.0f, 1.0f, 0.0f, 0.1f);

    pool.size[index] = 10.0f;
    pool.start_size[index] = 10.0f;
    pool.end_size[index] = 5.0f;

    pool.start_life[index] = 1000 + life;
    pool.life[index] = 1000 + life;
}

void mono::DefaultUpdater(struct ParticlePoolComponent& pool, size_t count, uint32_t delta_ms)
{
    const float delta_seconds = float(delta_ms) / 1000.0f;

    for(size_t index = 0; index < count; ++index)
    {
        const float t = 1.0f - float(pool.life[index]) / float(pool.start_life[index]);

        pool.position[index] += pool.velocity[index] * delta_seconds;
        pool.color[index] = mono::Color::Lerp(pool.start_color[index], pool.end_color[index], t);
        pool.size[index] = (1.0f - t) * pool.start_size[index] + t * pool.end_size[index];
    }
}


ParticleSystem::ParticleSystem(size_t count, size_t n_emitters)
    : m_particle_pools(count)
    , m_particle_drawers(count)
    , m_active_pools(count, false)
    , m_particle_emitters(n_emitters)
    , m_particle_pools_emitters(count)
{ }

ParticleSystem::~ParticleSystem()
{

}

uint32_t ParticleSystem::Id() const
{
    return mono::Hash(Name());
}

const char* ParticleSystem::Name() const
{
    return "ParticleSystem";
}

void ParticleSystem::Update(const mono::UpdateContext& update_context)
{
    for(size_t active_pool_index = 0; active_pool_index < m_active_pools.size(); ++active_pool_index)
    {
        const bool is_active = m_active_pools[active_pool_index];
        if(!is_active)
            continue;

        ParticlePoolComponent& pool_component = m_particle_pools[active_pool_index];
        pool_component.update_function(pool_component, pool_component.count_alive, update_context.delta_ms);

        for(size_t particle_index = 0; particle_index < pool_component.count_alive; ++particle_index)
        {
            int& life = pool_component.life[particle_index];
            life -= update_context.delta_ms;

            if(life <= 0 && pool_component.count_alive > 0)
            {
                --pool_component.count_alive;
                Swap(pool_component, particle_index, pool_component.count_alive);
            }
        }

        std::vector<ParticleEmitterComponent*>& pool_emitters = m_particle_pools_emitters[active_pool_index];
        for(ParticleEmitterComponent* emitter : pool_emitters)
            UpdateEmitter(emitter, pool_component, active_pool_index, update_context);
    }
}

void ParticleSystem::Sync()
{
    for(const DeferredReleasEmitter& deferred_release : m_deferred_release_emitter)
        ReleaseEmitter(deferred_release.pool_id, deferred_release.emitter);

    m_deferred_release_emitter.clear();
}

void ParticleSystem::UpdateEmitter(ParticleEmitterComponent* emitter, ParticlePoolComponent& particle_pool, uint32_t pool_id, const mono::UpdateContext& update_context)
{
    if(IsDone(*emitter))
    {
        if(emitter->type == EmitterType::BURST_REMOVE_ON_FINISH)
            m_deferred_release_emitter.push_back({pool_id, emitter});
        return;
    }

    const float delta_in_seconds = float(update_context.delta_ms) / 1000.0f;
    emitter->elapsed_time += delta_in_seconds;

    size_t new_particles = 0;
    if(emitter->type == EmitterType::BURST || emitter->type == EmitterType::BURST_REMOVE_ON_FINISH)
    {
        new_particles = emitter->emit_rate * emitter->duration;
    }
    else
    {
        new_particles = static_cast<size_t>(delta_in_seconds * (emitter->emit_rate + emitter->carry_over));
        if(new_particles == 0)
            emitter->carry_over += emitter->emit_rate;
        else
            emitter->carry_over = 0.0f;
    }

    const size_t start_index = particle_pool.count_alive;
    const size_t end_index = std::min(start_index + new_particles, particle_pool.pool_size -1);

    for(size_t index = start_index; index < end_index; ++index)
        emitter->generator(emitter->position, particle_pool, index);

    for(size_t index = start_index; index < end_index; ++index)
        WakeParticle(particle_pool, index);

    emitter->burst_emitted = true;
}

ParticlePoolComponent* ParticleSystem::AllocatePool(uint32_t id, size_t pool_size, ParticleUpdater update_function)
{
    assert(m_active_pools[id] == false);

    ParticlePoolComponent& particle_pool = m_particle_pools[id];

    particle_pool.position.resize(pool_size);
    particle_pool.velocity.resize(pool_size);
    particle_pool.rotation.resize(pool_size);
    particle_pool.angular_velocity.resize(pool_size);
    particle_pool.color.resize(pool_size);
    particle_pool.start_color.resize(pool_size);
    particle_pool.end_color.resize(pool_size);
    particle_pool.size.resize(pool_size);
    particle_pool.start_size.resize(pool_size);
    particle_pool.end_size.resize(pool_size);
    particle_pool.start_life.resize(pool_size);
    particle_pool.life.resize(pool_size);

    particle_pool.pool_size = pool_size;
    particle_pool.count_alive = 0;
    particle_pool.update_function = update_function;

    m_active_pools[id] = true;

    return &particle_pool;
}

void ParticleSystem::ReleasePool(uint32_t id)
{
    assert(m_active_pools[id]);

    std::vector<ParticleEmitterComponent*>& attached_emitters = m_particle_pools_emitters[id];
    for(ParticleEmitterComponent* emitter : attached_emitters)
        m_particle_emitters.ReleasePoolData(emitter);

    attached_emitters.clear();

    ParticleDrawerComponent& draw_component = m_particle_drawers[id];
    draw_component.texture = nullptr;

    m_active_pools[id] = false;
}

ParticlePoolComponent* ParticleSystem::GetPool(uint32_t id)
{
    assert(m_active_pools[id]);

    ParticlePoolComponent& particle_pool = m_particle_pools[id];
    return &particle_pool;
}

void ParticleSystem::SetPoolDrawData(uint32_t pool_id, mono::ITexturePtr texture, mono::BlendMode blend_mode)
{
    ParticleDrawerComponent& draw_component = m_particle_drawers[pool_id];
    draw_component.texture = texture;
    draw_component.blend_mode = blend_mode;
}

ParticleEmitterComponent* ParticleSystem::AttachEmitter(
    uint32_t pool_id, const math::Vector& position, float duration, float emit_rate, EmitterType emitter_type, ParticleGenerator generator)
{
    ParticleEmitterComponent* emitter = m_particle_emitters.GetPoolData();

    emitter->position = position;
    emitter->duration = duration;
    emitter->elapsed_time = 0.0f;
    emitter->carry_over = 0.0f;
    emitter->emit_rate = emit_rate;
    emitter->burst_emitted = false;
    emitter->type = emitter_type;
    emitter->generator = generator;

    m_particle_pools_emitters[pool_id].push_back(emitter);

    return emitter;
}

void ParticleSystem::ReleaseEmitter(uint32_t pool_id, ParticleEmitterComponent* emitter)
{
    m_particle_emitters.ReleasePoolData(emitter);
    mono::remove(m_particle_pools_emitters[pool_id], emitter);
}

void ParticleSystem::SetEmitterPosition(ParticleEmitterComponent* emitter, const math::Vector& position)
{

}

ParticleSystemStats ParticleSystem::GetStats() const
{
    ParticleSystemStats stats;
    stats.active_pools = std::count(m_active_pools.begin(), m_active_pools.end(), true);
    stats.active_emitters = m_particle_emitters.Used();

    return stats;
}
