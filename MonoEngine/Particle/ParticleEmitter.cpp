
#include "ParticleEmitter.h"
#include "ParticlePool.h"
#include "IRenderer.h"
#include "Math/Quad.h"

#include <limits>

using namespace mono;

ParticleEmitter::ParticleEmitter(const Configuration& config, ParticlePool& pool)
    : m_config(config),
      m_pool(pool)
{ }

void ParticleEmitter::doUpdate(unsigned int delta)
{
    const size_t new_particles = static_cast<size_t>(delta * m_config.emit_rate);
    const size_t start_index = m_pool.m_countAlive;
    const size_t end_index = std::min(start_index + new_particles, m_pool.m_poolSize -1);

    for(size_t index = start_index; index < end_index; ++index)
        m_config.generator(m_config.position, m_pool, index);

    for(size_t index = start_index; index < end_index; ++index)
        m_pool.Wake(index);

    m_config.updater(m_pool, m_pool.m_countAlive, delta);

    for(size_t index = 0; index < m_pool.m_countAlive; ++index)
    {
        int& life = m_pool.m_life[index];
        life -= delta;
        if(life < 0.0f)
            m_pool.Kill(index);
    }
}

void ParticleEmitter::doDraw(mono::IRenderer& renderer) const
{
    renderer.DrawPoints(m_pool.m_position, m_pool.m_color, m_pool.m_countAlive, m_config.point_size, m_config.texture);
}

math::Quad ParticleEmitter::BoundingBox() const
{
    constexpr float inf = std::numeric_limits<float>::infinity();
    return math::Quad(-inf, -inf, inf, inf);
}
