
#include "ParticleSystemDrawer.h"
#include "ParticleSystem.h"
#include "Rendering/IRenderer.h"
#include "Rendering/RenderBuffer/IRenderBuffer.h"
#include "Rendering/RenderBuffer/BufferFactory.h"
#include "Math/Quad.h"

using namespace mono;

ParticleSystemDrawer::ParticleSystemDrawer(const mono::ParticleSystem* particle_system)
    : m_particle_system(particle_system)
{ }

ParticleSystemDrawer::~ParticleSystemDrawer()
{ }

void ParticleSystemDrawer::Draw(mono::IRenderer& renderer) const
{
    const auto callback = [this, &renderer](uint32_t pool_index, const ParticlePoolComponent& pool, const ParticleDrawerComponent& drawer) {

        auto it = m_render_data.find(pool_index);
        if(it == m_render_data.end())
        {
            InternalRenderData new_render_data;
            new_render_data.position_buffer = mono::CreateRenderBuffer(BufferType::DYNAMIC, BufferData::FLOAT, 2, pool.pool_size, nullptr);
            new_render_data.rotation_buffer = mono::CreateRenderBuffer(BufferType::DYNAMIC, BufferData::FLOAT, 1, pool.pool_size, nullptr);
            new_render_data.color_buffer = mono::CreateRenderBuffer(BufferType::DYNAMIC, BufferData::FLOAT, 4, pool.pool_size, nullptr);
            new_render_data.point_size_buffer = mono::CreateRenderBuffer(BufferType::DYNAMIC, BufferData::FLOAT, 1, pool.pool_size, nullptr);

            it = m_render_data.insert(std::make_pair(pool_index, std::move(new_render_data))).first;
        }

        if(pool.count_alive == 0)
            return;

        it->second.position_buffer->UpdateData(pool.position.data(), 0, pool.count_alive);
        it->second.rotation_buffer->UpdateData(pool.rotation.data(), 0, pool.count_alive);
        it->second.color_buffer->UpdateData(pool.color.data(), 0, pool.count_alive);
        it->second.point_size_buffer->UpdateData(pool.size.data(), 0, pool.count_alive);

        renderer.DrawParticlePoints(
            it->second.position_buffer.get(),
            it->second.rotation_buffer.get(),
            it->second.color_buffer.get(),
            it->second.point_size_buffer.get(),
            drawer.texture.get(),
            drawer.blend_mode,
            pool.count_alive);
    };

    m_particle_system->ForEach(callback);
}

math::Quad ParticleSystemDrawer::BoundingBox() const
{
    return math::InfQuad;
}
