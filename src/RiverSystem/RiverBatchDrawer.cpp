
#include "RiverBatchDrawer.h"
#include "RiverSystem.h"
#include "Paths/PathSystem.h"
#include "Paths/PathFactory.h"
#include "TransformSystem/TransformSystem.h"

#include "Rendering/RenderSystem.h"
#include "Rendering/IRenderer.h"
#include "Rendering/RenderBuffer/IRenderBuffer.h"
#include "Rendering/Texture/ITexture.h"
#include "Rendering/Texture/ITextureFactory.h"

using namespace mono;


RiverBatchDrawer::RiverBatchDrawer(const RiverSystem* river_system, mono::PathSystem* path_system, const mono::TransformSystem* transform_system)
    : m_river_system(river_system)
    , m_path_system(path_system)
    , m_transform_system(transform_system)
{
    const auto dirty_callback = [this](uint32_t entity_id) {
        auto it = m_cached_rivers.find(entity_id);
        if(it != m_cached_rivers.end())
            it->second.dirty = true;
    };
    m_callback_id = path_system->RegisterDirtyCallback(dirty_callback);
}

RiverBatchDrawer::~RiverBatchDrawer()
{
    m_path_system->RemoveDirtyCallback(m_callback_id);
}

void RiverBatchDrawer::Draw(mono::IRenderer& renderer) const
{
    const auto collect_rivers = [this, &renderer](uint32_t entity_id, const RiverComponent& component) {
        auto it = m_cached_rivers.find(entity_id);

        if(it == m_cached_rivers.end() || (it != m_cached_rivers.end() && NeedsUpdate(it->second, component)))
            it = m_cached_rivers.insert_or_assign(entity_id, CacheRiverData(entity_id, component)).first;

        if(!it->second.texture || !it->second.buffers.vertices)
            return;

        const math::Matrix& world_transform = m_transform_system->GetWorld(entity_id);
        const auto scope = mono::MakeTransformScope(world_transform, &renderer);

        const CachedRiver& river = it->second;
        renderer.DrawRiver(
            river.buffers.vertices.get(),
            river.buffers.anotations.get(),
            river.buffers.indices.get(),
            river.texture.get(),
            component.color,
            component.edge_fade,
            0,
            river.buffers.indices->Size());
    };

    m_river_system->ForEeach(collect_rivers);
}

math::Quad RiverBatchDrawer::BoundingBox() const
{
    return math::InfQuad;
}

RiverBatchDrawer::CachedRiver RiverBatchDrawer::CacheRiverData(uint32_t entity_id, const RiverComponent& component) const
{
    const mono::PathComponent* path = m_path_system->GetPath(entity_id);

    mono::PathOptions path_options;
    path_options.width = component.width;
    path_options.color = component.color;
    path_options.uv_mode = mono::UVMode(mono::UVMode::DISTANCE | mono::UVMode::NORMALIZED_WIDTH);
    path_options.closed = false;

    CachedRiver cached_river;
    cached_river.dirty = false;
    cached_river.width = component.width;
    cached_river.texture_name = component.texture_name;
    cached_river.texture = mono::RenderSystem::GetTextureFactory()->CreateTexture(component.texture_name.c_str());

    if(mono::ValidatePathParameters(path->type, path->points))
        cached_river.buffers = mono::BuildPathDrawBuffers(path->type, path->points, path_options);

    return cached_river;
}

bool RiverBatchDrawer::NeedsUpdate(const CachedRiver& river, const RiverComponent& component) const
{
    return
        river.dirty ||
        river.width != component.width ||
        river.texture_name != component.texture_name;
}
