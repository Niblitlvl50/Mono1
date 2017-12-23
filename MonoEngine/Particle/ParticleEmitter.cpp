
#include "ParticleEmitter.h"
#include "ParticlePool.h"

#include <stdlib.h>

using namespace mono;

ParticleEmitter::ParticleEmitter(const Configuration& config, ParticlePool& pool)
    : m_config(config),
      m_position(config.position),
      m_pool(pool)
{ }

void ParticleEmitter::SetPosition(const math::Vector& position)
{
    m_position = position;
}

void ParticleEmitter::doUpdate(unsigned int delta)
{
    const size_t new_particles = std::max(static_cast<size_t>(float(delta) * m_config.emit_rate), size_t(1));
    const size_t start_index = m_pool.m_countAlive;
    const size_t end_index = std::min(start_index + new_particles, m_pool.m_poolSize -1);

    for(size_t index = start_index; index < end_index; ++index)
        m_config.generator(m_position, m_pool, index);

    for(size_t index = start_index; index < end_index; ++index)
        m_pool.Wake(index);
}
