
#include "ParticleSystemDrawer.h"
#include "ParticleSystem.h"
#include "Rendering/IRenderer.h"
#include "Rendering/IRenderBuffer.h"
#include "Rendering/BufferFactory.h"
#include "Math/Quad.h"

using namespace mono;

ParticleSystemDrawer::ParticleSystemDrawer(const mono::ParticleSystem* particle_system)
    : m_particle_system(particle_system)
{ }

ParticleSystemDrawer::~ParticleSystemDrawer()
{ }

void ParticleSystemDrawer::doDraw(mono::IRenderer& renderer) const
{
    const auto callback = [this, &renderer](uint32_t pool_index, const ParticlePoolComponent& pool, const ParticleDrawerComponent& drawer) {

        auto it = m_render_data.find(pool_index);
        if(it == m_render_data.end())
        {
            InternalRenderData new_render_data;
            new_render_data.position_buffer = mono::CreateRenderBuffer(BufferTarget::ARRAY_BUFFER, BufferType::DYNAMIC, BufferData::FLOAT, pool.pool_size * 2);
            new_render_data.rotation_buffer = mono::CreateRenderBuffer(BufferTarget::ARRAY_BUFFER, BufferType::DYNAMIC, BufferData::FLOAT, pool.pool_size);
            new_render_data.color_buffer = mono::CreateRenderBuffer(BufferTarget::ARRAY_BUFFER, BufferType::DYNAMIC, BufferData::FLOAT, pool.pool_size * 4); 
            new_render_data.point_size_buffer = mono::CreateRenderBuffer(BufferTarget::ARRAY_BUFFER, BufferType::DYNAMIC, BufferData::FLOAT, pool.pool_size);

            it = m_render_data.insert(std::make_pair(pool_index, std::move(new_render_data))).first;
        }

        it->second.position_buffer->UpdateData(pool.position.data(), 0, pool.count_alive * 2);
        it->second.rotation_buffer->UpdateData(pool.rotation.data(), 0, pool.count_alive);
        it->second.color_buffer->UpdateData(pool.color.data(), 0, pool.count_alive * 4);
        it->second.point_size_buffer->UpdateData(pool.size.data(), 0, pool.count_alive);

        renderer.DrawParticlePoints(
            it->second.position_buffer.get(),
            it->second.rotation_buffer.get(),
            it->second.color_buffer.get(),
            it->second.point_size_buffer.get(),
            drawer.texture,
            drawer.blend_mode,
            pool.count_alive);
    };

    m_particle_system->ForEach(callback);
}

math::Quad ParticleSystemDrawer::BoundingBox() const
{
    return math::InfQuad;
}
