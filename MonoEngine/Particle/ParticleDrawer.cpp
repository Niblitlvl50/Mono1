
#include "ParticleDrawer.h"
#include "ParticlePool.h"

#include "Rendering/IRenderer.h"
#include "Rendering/BufferFactory.h"
#include "Math/Quad.h"

using namespace mono;

ParticleDrawer::ParticleDrawer(const Configuration& config, ParticlePool& pool)
    : m_config(config),
      m_pool(pool)
{
    const size_t pool_size = m_pool.m_poolSize;

    m_positionBuffer = mono::CreateRenderBuffer(BufferType::DYNAMIC, BufferData::FLOAT, pool_size * 2);
    m_colorBuffer = mono::CreateRenderBuffer(BufferType::DYNAMIC, BufferData::FLOAT, pool_size * 4); 

    ClearRenderBuffer();    
}

ParticleDrawer::~ParticleDrawer()
{ }

void ParticleDrawer::doDraw(mono::IRenderer& renderer) const
{
    m_positionBuffer->UpdateData(m_pool.m_position.data(), 0, m_pool.m_countAlive * 2);
    m_colorBuffer->UpdateData(m_pool.m_color.data(), 0, m_pool.m_countAlive * 4);

    renderer.DrawParticlePoints(m_positionBuffer.get(), m_colorBuffer.get(), m_config.point_size, m_config.texture, m_pool.m_countAlive);    
}

math::Quad ParticleDrawer::BoundingBox() const
{
    constexpr float inf = std::numeric_limits<float>::infinity();
    return math::Quad(-inf, -inf, inf, inf);    
}
