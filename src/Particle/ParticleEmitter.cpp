
#include "ParticleEmitter.h"
#include "ParticlePool.h"

#include <stdlib.h>

using namespace mono;

ParticleEmitter::ParticleEmitter(const Configuration& config, ParticlePool* pool)
    : m_config(config)
    , m_burst_emitted(false)
    , m_duration(0.0f)
    , m_carry_over(0.0f)
    , m_position(config.position)
    , m_pool(pool)
{ }

void ParticleEmitter::SetPosition(const math::Vector& position)
{
    m_position = position;
}

void ParticleEmitter::Stop()
{
    m_duration = m_config.duration;
}

bool ParticleEmitter::IsDone() const
{
    if(m_config.burst)
        return m_burst_emitted;

    return (m_config.duration > 0.0f && m_duration > m_config.duration);
}

void ParticleEmitter::doUpdate(const UpdateContext& update_context)
{
    if(IsDone())
        return;

    m_duration += float(update_context.delta_ms) / 1000.0f;

    size_t new_particles = 0;
    
    if(m_config.burst)
    {
        new_particles = m_config.emit_rate * m_config.duration;
    }
    else
    {
        const float delta_second = float(update_context.delta_ms) / 1000.0f;
        new_particles = static_cast<size_t>(delta_second * (m_config.emit_rate + m_carry_over));
        if(new_particles == 0)
            m_carry_over += m_config.emit_rate;
        else
            m_carry_over = 0.0f;
    }

    const size_t start_index = m_pool->m_count_alive;
    const size_t end_index = std::min(start_index + new_particles, m_pool->m_pool_size -1);

    for(size_t index = start_index; index < end_index; ++index)
        m_config.generator(m_position, *m_pool, index);

    for(size_t index = start_index; index < end_index; ++index)
        m_pool->Wake(index);

    m_burst_emitted = true;
}
