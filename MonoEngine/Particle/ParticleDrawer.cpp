
#include "ParticleDrawer.h"
#include "ParticlePool.h"

#include "Rendering/IRenderer.h"
#include "Rendering/BufferFactory.h"
#include "Math/Quad.h"

#include <limits>

using namespace mono;

ParticleDrawer::ParticleDrawer(const mono::ITexturePtr& texture, mono::BlendMode blend_mode, ParticlePool& pool)
    : m_texture(texture)
    , m_blend_mode(blend_mode)
    , m_pool(pool)
{
    const size_t pool_size = m_pool.m_pool_size;

    m_positionBuffer = mono::CreateRenderBuffer(BufferTarget::ARRAY_BUFFER, BufferType::DYNAMIC, BufferData::FLOAT, pool_size * 2);
    m_colorBuffer = mono::CreateRenderBuffer(BufferTarget::ARRAY_BUFFER, BufferType::DYNAMIC, BufferData::FLOAT, pool_size * 4); 
    m_point_size_buffer = mono::CreateRenderBuffer(BufferTarget::ARRAY_BUFFER, BufferType::DYNAMIC, BufferData::FLOAT, pool_size);

    ClearRenderBuffer();
}

ParticleDrawer::~ParticleDrawer()
{ }

void ParticleDrawer::doDraw(mono::IRenderer& renderer) const
{
    m_positionBuffer->UpdateData(m_pool.m_position.data(), 0, m_pool.m_count_alive * 2);
    m_colorBuffer->UpdateData(m_pool.m_color.data(), 0, m_pool.m_count_alive * 4);
    m_point_size_buffer->UpdateData(m_pool.m_size.data(), 0, m_pool.m_count_alive);

    renderer.DrawParticlePoints(
        m_positionBuffer.get(), m_colorBuffer.get(), m_point_size_buffer.get(), m_texture, m_blend_mode, m_pool.m_count_alive);
}

math::Quad ParticleDrawer::BoundingBox() const
{
    constexpr float inf = std::numeric_limits<float>::infinity();
    return math::Quad(-inf, -inf, inf, inf);    
}
