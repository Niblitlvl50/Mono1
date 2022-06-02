
#include "ParticleSystemDrawer.h"
#include "ParticleSystem.h"
#include "Rendering/IRenderer.h"
#include "Rendering/RenderBuffer/IRenderBuffer.h"
#include "Rendering/RenderBuffer/BufferFactory.h"
#include "TransformSystem/TransformSystem.h"
#include "Math/Quad.h"

#include <iterator>

using namespace mono;

ParticleSystemDrawer::ParticleSystemDrawer(
    const mono::ParticleSystem* particle_system, const mono::TransformSystem* transform_system, mono::ParticleDrawLayer draw_layer)
    : m_particle_system(particle_system)
    , m_transform_system(transform_system)
    , m_draw_layer(draw_layer)
{ }

ParticleSystemDrawer::~ParticleSystemDrawer()
{ }

void ParticleSystemDrawer::Draw(mono::IRenderer& renderer) const
{
    std::vector<uint32_t> active_particles;

    const auto callback = [this, &renderer, &active_particles](uint32_t pool_index, const ParticlePoolComponent& pool, const ParticleDrawerComponent& drawer)
    {
        if(drawer.draw_layer != m_draw_layer)
            return;

        active_particles.push_back(pool_index);

        auto it = m_render_data.find(pool_index);
        if(it == m_render_data.end() || it->second.position_buffer->Size() < pool.pool_size)
        {
            InternalRenderData new_render_data;
            new_render_data.position_buffer = mono::CreateRenderBuffer(BufferType::DYNAMIC, BufferData::FLOAT, 2, pool.pool_size, nullptr);
            new_render_data.rotation_buffer = mono::CreateRenderBuffer(BufferType::DYNAMIC, BufferData::FLOAT, 1, pool.pool_size, nullptr);
            new_render_data.color_buffer = mono::CreateRenderBuffer(BufferType::DYNAMIC, BufferData::FLOAT, 4, pool.pool_size, nullptr);
            new_render_data.point_size_buffer = mono::CreateRenderBuffer(BufferType::DYNAMIC, BufferData::FLOAT, 1, pool.pool_size, nullptr);

            it = m_render_data.insert_or_assign(pool_index, std::move(new_render_data)).first;
        }

        if(pool.count_alive == 0 || drawer.texture == nullptr)
            return;

        it->second.position_buffer->UpdateData(pool.position.data(), 0, pool.count_alive);
        it->second.rotation_buffer->UpdateData(pool.rotation.data(), 0, pool.count_alive);
        it->second.color_buffer->UpdateData(pool.color.data(), 0, pool.count_alive);
        it->second.point_size_buffer->UpdateData(pool.size.data(), 0, pool.count_alive);

        const math::Matrix& transform = (drawer.transform_space == ParticleTransformSpace::LOCAL) ? m_transform_system->GetWorld(pool_index) : math::Matrix();
        const auto transform_scope = mono::MakeTransformScope(transform, &renderer);

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

    std::vector<uint32_t> diff_result;
    std::set_difference(
        m_last_active_particles.begin(),
        m_last_active_particles.end(),
        active_particles.begin(),
        active_particles.end(),
        std::back_inserter(diff_result));

    for(uint32_t id : diff_result)
        m_render_data.erase(id);

    m_last_active_particles = active_particles;
}

math::Quad ParticleSystemDrawer::BoundingBox() const
{
    return math::InfQuad;
}
