
#include "ParticleEmitter.h"
#include "ParticlePool.h"
#include "IRenderer.h"
#include "Math/Quad.h"

#include "Rendering/BufferFactory.h"

#include <limits>

using namespace mono;

ParticleEmitter::ParticleEmitter(const Configuration& config, ParticlePool& pool)
    : m_config(config),
      m_position(config.position),
      m_pool(pool)
{
    m_positionBuffer = mono::CreateRenderBuffer(BufferType::DYNAMIC, BufferData::FLOAT);
    m_positionBuffer->Initialize(m_pool.m_poolSize * 2);

    m_colorBuffer = mono::CreateRenderBuffer(BufferType::DYNAMIC, BufferData::FLOAT); 
    m_colorBuffer->Initialize(m_pool.m_poolSize * 4);

    ClearRenderBuffer();
}

ParticleEmitter::~ParticleEmitter()
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
    m_positionBuffer->UpdateData(m_pool.m_position.data(), 0, m_pool.m_countAlive * 2);
    m_colorBuffer->UpdateData(m_pool.m_color.data(), 0, m_pool.m_countAlive * 4);

    renderer.DrawParticlePoints(m_positionBuffer.get(), m_colorBuffer.get(), m_config.point_size, m_config.texture, m_pool.m_countAlive);
}

math::Quad ParticleEmitter::BoundingBox() const
{
    constexpr float inf = std::numeric_limits<float>::infinity();
    return math::Quad(-inf, -inf, inf, inf);
}
