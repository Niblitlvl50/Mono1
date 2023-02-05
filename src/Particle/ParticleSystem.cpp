
#include "ParticleSystem.h"
#include "Rendering/RenderBuffer/IRenderBuffer.h"
#include "Rendering/Texture/ITextureFactory.h"
#include "Rendering/RenderSystem.h"
#include "System/Hash.h"
#include "System/Debug.h"
#include "TransformSystem/TransformSystem.h"
#include "Util/Algorithm.h"
#include "Util/Random.h"


using namespace mono;

namespace
{
    void Swap(ParticlePoolComponent& pool_component, uint32_t first, uint32_t second)
    {
        std::swap(pool_component.position[first],           pool_component.position[second]);
        std::swap(pool_component.velocity[first],           pool_component.velocity[second]);
        std::swap(pool_component.rotation[first],           pool_component.rotation[second]);
        std::swap(pool_component.angular_velocity[first],   pool_component.angular_velocity[second]);
        std::swap(pool_component.color[first],              pool_component.color[second]);
        std::swap(pool_component.gradient[first],           pool_component.gradient[second]);
        std::swap(pool_component.start_life[first],         pool_component.start_life[second]);
        std::swap(pool_component.life[first],               pool_component.life[second]);
        std::swap(pool_component.size[first],               pool_component.size[second]);
    }

    void WakeParticle(ParticlePoolComponent& pool_component, uint32_t index)
    {
        if(pool_component.count_alive < pool_component.pool_size)
        {
            Swap(pool_component, index, pool_component.count_alive);
            ++pool_component.count_alive;
        }
    }

    mono::ParticlePoolComponentView MakeViewFromPool(mono::ParticlePoolComponent& particle_pool, uint32_t index)
    {
        return {
            particle_pool.position[index],
            particle_pool.velocity[index],

            particle_pool.rotation[index],
            particle_pool.angular_velocity[index],

            particle_pool.color[index],
            particle_pool.gradient[index],

            particle_pool.size[index],
            particle_pool.start_size[index],
            particle_pool.end_size[index],

            particle_pool.life[index],
            particle_pool.start_life[index],
        };
    }
}

void mono::DefaultGenerator(const ParticleGeneratorContext& context, ParticlePoolComponentView& particle_view)
{
    const float x = mono::Random(-2.0f, 2.0f);
    const float y = mono::Random(0.5f, 4.0f);
    const float life = mono::Random(0.0f, 0.5f);

    particle_view.position = context.position;
    particle_view.velocity = math::Vector(x, y);
    particle_view.rotation = 0.0f;
    particle_view.angular_velocity = 0.0f;

    particle_view.gradient = mono::Color::MakeGradient<4>(
        { 0.0f, 0.25f, 0.5f, 1.0f },
        { mono::Color::RED, mono::Color::GREEN, mono::Color::BLUE, mono::Color::WHITE }
    );

    particle_view.size = 32.0f;
    particle_view.start_size = 32.0f;
    particle_view.end_size = 24.0f;

    particle_view.start_life = 1.0f + life;
    particle_view.life = 1.0f + life;
}

void mono::DefaultUpdater(ParticlePoolComponentView& component_view, float delta_s)
{
    const float t = 1.0f - float(component_view.life) / float(component_view.start_life);

    component_view.position += component_view.velocity * delta_s;
    component_view.color = mono::Color::ColorFromGradient(component_view.gradient, t);
    component_view.size = (1.0f - t) * component_view.start_size + t * component_view.end_size;
    component_view.rotation += component_view.angular_velocity * delta_s;
}


ParticleSystem::ParticleSystem(uint32_t count, uint32_t n_emitters, const mono::TransformSystem* transform_system)
    : m_transform_system(transform_system)
    , m_particle_pools(count)
    , m_particle_drawers(count)
    , m_active_pools(count, false)
    , m_particle_emitters(n_emitters)
    , m_particle_pools_emitters(count)
{ }

ParticleSystem::~ParticleSystem()
{ }

const char* ParticleSystem::Name() const
{
    return "ParticleSystem";
}

void ParticleSystem::Update(const mono::UpdateContext& update_context)
{
    for(uint32_t active_pool_index = 0; active_pool_index < m_active_pools.size(); ++active_pool_index)
    {
        const bool is_active = m_active_pools[active_pool_index];
        if(!is_active)
            continue;

        ParticlePoolComponent& pool_component = m_particle_pools[active_pool_index];
        const ParticleDrawerComponent& drawer_component = m_particle_drawers[active_pool_index];

        std::vector<ParticleEmitterComponent*>& pool_emitters = m_particle_pools_emitters[active_pool_index];
        for(ParticleEmitterComponent* emitter : pool_emitters)
        {
            const bool emitter_finished = IsEmitterFinished(emitter);
            if(emitter_finished)
            {
                if(emitter->type == EmitterType::BURST_REMOVE_ON_FINISH)
                    m_deferred_release_emitter.push_back({active_pool_index, emitter});
            }
            else
            {
                UpdateEmitter(active_pool_index, emitter, pool_component, drawer_component.transform_space, update_context);
            }
        }

        for(uint32_t index = 0; index < pool_component.count_alive; ++index)
        {
            math::Vector& velocity = pool_component.velocity[index];
            velocity *= (1.0f - pool_component.particle_damping);

            ParticlePoolComponentView view = MakeViewFromPool(pool_component, index);
            pool_component.update_function(view, update_context.delta_s);
        }

        for(uint32_t index = 0; index < pool_component.count_alive; ++index)
        {
            float& life = pool_component.life[index];
            life -= update_context.delta_s;

            if(life <= 0 && pool_component.count_alive > 0)
            {
                --pool_component.count_alive;
                Swap(pool_component, index, pool_component.count_alive);
            }
        }
    }
}

void ParticleSystem::Sync()
{
    for(const DeferredReleasEmitter& deferred_release : m_deferred_release_emitter)
        ReleaseEmitter(deferred_release.pool_id, deferred_release.emitter);

    m_deferred_release_emitter.clear();
}

void ParticleSystem::UpdateEmitter(
    uint32_t entity_id,
    ParticleEmitterComponent* emitter,
    ParticlePoolComponent& particle_pool,
    ParticleTransformSpace transform_space,
    const mono::UpdateContext& update_context)
{
    const math::Vector world_position = m_transform_system->GetWorldPosition(entity_id);
    const math::Vector emitter_position =
        (transform_space == ParticleTransformSpace::LOCAL) ? emitter->position : world_position;

    emitter->elapsed_time += update_context.delta_s;

    uint32_t new_particles = 0;
    if(emitter->type == EmitterType::BURST || emitter->type == EmitterType::BURST_REMOVE_ON_FINISH)
    {
        new_particles = emitter->emit_rate * emitter->duration;
    }
    else
    {
        new_particles = static_cast<uint32_t>(update_context.delta_s * (emitter->emit_rate + emitter->carry_over));
        if(new_particles == 0)
            emitter->carry_over += emitter->emit_rate;
        else
            emitter->carry_over = 0.0f;
    }

    const uint32_t start_index = particle_pool.count_alive;
    const uint32_t end_index = std::min(start_index + new_particles, particle_pool.pool_size -1);

    ParticleGeneratorContext generator_context;
    generator_context.position = emitter_position;
    generator_context.particles_to_emit = end_index - start_index;

    for(uint32_t index = start_index; index < end_index; ++index)
    {
        generator_context.particle_index = index - start_index;
        ParticlePoolComponentView view = MakeViewFromPool(particle_pool, index);
        emitter->generator(generator_context, view);
    }

    for(uint32_t index = start_index; index < end_index; ++index)
        WakeParticle(particle_pool, index);

    emitter->burst_emitted = true;
}

ParticlePoolComponent* ParticleSystem::AllocatePool(uint32_t id)
{
    return AllocatePool(id, 10, DefaultUpdater);
}

ParticlePoolComponent* ParticleSystem::AllocatePool(uint32_t id, uint32_t pool_size, ParticleUpdater update_function)
{
    MONO_ASSERT(m_active_pools[id] == false);

    ParticlePoolComponent& particle_pool = m_particle_pools[id];

    particle_pool.position.resize(pool_size);
    particle_pool.velocity.resize(pool_size);
    particle_pool.rotation.resize(pool_size);
    particle_pool.angular_velocity.resize(pool_size);
    particle_pool.color.resize(pool_size);
    particle_pool.gradient.resize(pool_size);
    particle_pool.size.resize(pool_size);
    particle_pool.start_size.resize(pool_size);
    particle_pool.end_size.resize(pool_size);
    particle_pool.start_life.resize(pool_size);
    particle_pool.life.resize(pool_size);

    particle_pool.pool_size = pool_size;
    particle_pool.count_alive = 0;
    particle_pool.update_function = update_function;
    particle_pool.particle_damping = 0.0f;

    m_active_pools[id] = true;

    return &particle_pool;
}

void ParticleSystem::ReleasePool(uint32_t id)
{
    MONO_ASSERT(m_active_pools[id]);

    std::vector<ParticleEmitterComponent*>& attached_emitters = m_particle_pools_emitters[id];
    for(ParticleEmitterComponent* emitter : attached_emitters)
        m_particle_emitters.ReleasePoolData(emitter);

    attached_emitters.clear();

    ParticleDrawerComponent& draw_component = m_particle_drawers[id];
    draw_component.texture = nullptr;

    m_active_pools[id] = false;
}

void ParticleSystem::SetPoolData(
    uint32_t id,
    uint32_t pool_size,
    const char* texture_file,
    mono::BlendMode blend_mode,
    ParticleDrawLayer draw_layer,
    ParticleTransformSpace transform_space,
    float particle_damping,
    ParticleUpdater update_function)
{
    ParticlePoolComponent& particle_pool = m_particle_pools[id];

    particle_pool.position.resize(pool_size);
    particle_pool.velocity.resize(pool_size);
    particle_pool.rotation.resize(pool_size);
    particle_pool.angular_velocity.resize(pool_size);
    particle_pool.color.resize(pool_size);
    particle_pool.gradient.resize(pool_size);
    particle_pool.size.resize(pool_size);
    particle_pool.start_size.resize(pool_size);
    particle_pool.end_size.resize(pool_size);
    particle_pool.start_life.resize(pool_size);
    particle_pool.life.resize(pool_size);

    particle_pool.pool_size = pool_size;
    particle_pool.count_alive = 0;
    particle_pool.update_function = update_function;
    particle_pool.particle_damping = particle_damping;

    mono::ITexturePtr texture = mono::RenderSystem::GetTextureFactory()->CreateTexture(texture_file);
    SetPoolDrawData(id, texture, blend_mode, draw_layer, transform_space);
}

ParticlePoolComponent* ParticleSystem::GetPool(uint32_t id)
{
    MONO_ASSERT(m_active_pools[id]);

    ParticlePoolComponent& particle_pool = m_particle_pools[id];
    return &particle_pool;
}

void ParticleSystem::SetPoolDrawData(
    uint32_t pool_id, mono::ITexturePtr texture, mono::BlendMode blend_mode, ParticleDrawLayer draw_layer, ParticleTransformSpace transform_space)
{
    ParticleDrawerComponent& draw_component = m_particle_drawers[pool_id];
    draw_component.texture = texture;
    draw_component.blend_mode = blend_mode;
    draw_component.draw_layer = draw_layer;
    draw_component.transform_space = transform_space;
}

ParticleEmitterComponent* ParticleSystem::AttachEmitter(
    uint32_t pool_id, const math::Vector& position, float duration, float emit_rate, EmitterType emitter_type, ParticleGenerator generator)
{
    ParticleEmitterComponent* emitter = m_particle_emitters.GetPoolData();
    if(!emitter)
        return nullptr;

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

ParticleEmitterComponent* ParticleSystem::AttachAreaEmitter(
    uint32_t pool_id, float duration_seconds, float emit_rate, EmitterType emitter_type, const ParticleGeneratorProperties& generator_properties)
{
    ParticleEmitterComponent* emitter_component = AttachEmitter(pool_id, math::ZeroVec, duration_seconds, emit_rate, emitter_type, DefaultGenerator);
    SetGeneratorProperties(emitter_component, generator_properties);
    return emitter_component;
}

void ParticleSystem::ReleaseEmitter(uint32_t pool_id, ParticleEmitterComponent* emitter)
{
    m_particle_emitters.ReleasePoolData(emitter);
    mono::remove(m_particle_pools_emitters[pool_id], emitter);
}

void ParticleSystem::SetEmitterPosition(ParticleEmitterComponent* emitter, const math::Vector& position)
{
    emitter->position = position;
}

void ParticleSystem::SetGeneratorProperties(ParticleEmitterComponent* emitter, const ParticleGeneratorProperties& generator_properties)
{
    const bool valid_data =
        Validate(generator_properties.direction_degrees_interval) &&
        Validate(generator_properties.magnitude_interval) &&
        Validate(generator_properties.angular_velocity_interval) &&
        Validate(generator_properties.life_interval) &&
        Validate(generator_properties.start_size_spread) &&
        Validate(generator_properties.end_size_spread);

    MONO_ASSERT(valid_data);

    const ParticleGenerator generator = [generator_properties]
        (const ParticleGeneratorContext& context, ParticlePoolComponentView& component_view) {

        const math::Vector half_area = generator_properties.emit_area / 2.0f;
        const math::Vector offset = math::Vector(
            mono::Random(-half_area.x, half_area.x),
            mono::Random(-half_area.y, half_area.y)
        );

        float direction = 0;

        if(generator_properties.uniform_direction)
        {
            const float delta_interval
                = generator_properties.direction_degrees_interval.max - generator_properties.direction_degrees_interval.min;
            const float delta_direction = delta_interval / context.particles_to_emit;

            direction = generator_properties.direction_degrees_interval.min + (context.particle_index * delta_direction);
        }
        else
        {
            direction = mono::Random(generator_properties.direction_degrees_interval.min, generator_properties.direction_degrees_interval.max);
        }

        const float magnitude_variation = mono::Random(generator_properties.magnitude_interval.min, generator_properties.magnitude_interval.max);
        const math::Vector& velocity = math::VectorFromAngle(math::ToRadians(direction)) * magnitude_variation;

        component_view.position         = context.position + offset;
        component_view.velocity         = velocity;
        component_view.rotation         = 0.0f;
        component_view.angular_velocity = mono::Random(generator_properties.angular_velocity_interval.min, generator_properties.angular_velocity_interval.max);
        component_view.color            = generator_properties.color_gradient.color[0];
        component_view.gradient         = generator_properties.color_gradient;
        component_view.start_size       = generator_properties.start_size_spread.value
            + mono::Random(generator_properties.start_size_spread.spread.min, generator_properties.start_size_spread.spread.max);
        component_view.end_size         = generator_properties.end_size_spread.value
            + mono::Random(generator_properties.end_size_spread.spread.min, generator_properties.end_size_spread.spread.max);
        component_view.size             = component_view.start_size;
        component_view.life             = mono::Random(generator_properties.life_interval.min, generator_properties.life_interval.max);
        component_view.start_life       = component_view.life;
    };

    emitter->generator = generator;
}

void ParticleSystem::RestartEmitter(ParticleEmitterComponent* emitter)
{
    emitter->elapsed_time = 0.0f;
    emitter->carry_over = 0.0f;
    emitter->burst_emitted = false;
}

bool ParticleSystem::IsEmitterFinished(const ParticleEmitterComponent* emitter) const
{
    const bool is_burst = (emitter->type == EmitterType::BURST || emitter->type == EmitterType::BURST_REMOVE_ON_FINISH);
    if(is_burst && emitter->burst_emitted)
        return true;

    return (emitter->duration > 0.0f && emitter->elapsed_time >= emitter->duration);
}

const std::vector<ParticleEmitterComponent*>& ParticleSystem::GetAttachedEmitters(uint32_t pool_id) const
{
    return m_particle_pools_emitters[pool_id];
}

ParticleSystemStats ParticleSystem::GetStats() const
{
    ParticleSystemStats stats;
    stats.active_pools = std::count(m_active_pools.begin(), m_active_pools.end(), true);
    stats.active_emitters = m_particle_emitters.Used();

    return stats;
}
