
#include "RoadBatchDrawer.h"
#include "RoadSystem.h"
#include "Paths/PathSystem.h"
#include "Paths/PathFactory.h"
#include "TransformSystem/TransformSystem.h"

#include "Rendering/RenderSystem.h"
#include "Rendering/IRenderer.h"
#include "Rendering/RenderBuffer/IRenderBuffer.h"
#include "Rendering/Texture/ITexture.h"
#include "Rendering/Texture/ITextureFactory.h"

using namespace mono;

struct RoadBatchDrawer::CachedRoad
{
    bool dirty;
    float width;
    std::string texture_name;
    mono::ITexturePtr texture;
    mono::PathDrawBuffer buffers;
};

RoadBatchDrawer::RoadBatchDrawer(const RoadSystem* road_system, mono::PathSystem* path_system, const mono::TransformSystem* transform_system)
    : m_road_system(road_system)
    , m_path_system(path_system)
    , m_transform_system(transform_system)
{
    const auto dirty_callback = [this](uint32_t entity_id) {
        auto it = m_cached_roads.find(entity_id);
        if(it != m_cached_roads.end())
            it->second.dirty = true;
    };
    m_callback_id = path_system->RegisterDirtyCallback(dirty_callback);
}

RoadBatchDrawer::~RoadBatchDrawer()
{
    m_path_system->RemoveDirtyCallback(m_callback_id);
}

void RoadBatchDrawer::Draw(mono::IRenderer& renderer) const
{
    const auto collect_roads = [this, &renderer](uint32_t entity_id, const RoadComponent& component) {
        auto it = m_cached_roads.find(entity_id);

        if(it == m_cached_roads.end() || (it != m_cached_roads.end() && NeedsUpdate(it->second, component)))
            it = m_cached_roads.insert_or_assign(entity_id, CacheRoadData(entity_id, component)).first;

        if(!it->second.texture || !it->second.buffers.vertices)
            return;

        const math::Matrix& world_transform = m_transform_system->GetWorld(entity_id);
        const auto scope = mono::MakeTransformScope(world_transform, &renderer);

        const CachedRoad& road = it->second;
        renderer.DrawAnnotatedTrianges(
            road.buffers.vertices.get(),
            road.buffers.anotations.get(),
            road.buffers.indices.get(),
            road.texture.get(),
            mono::Color::WHITE,
            0,
            road.buffers.indices->Size());
    };

    m_road_system->ForEeach(collect_roads);
}

math::Quad RoadBatchDrawer::BoundingBox() const
{
    return math::InfQuad;
}

RoadBatchDrawer::CachedRoad RoadBatchDrawer::CacheRoadData(uint32_t entity_id, const RoadComponent& component) const
{
    const mono::PathComponent* path = m_path_system->GetPath(entity_id);

    mono::PathOptions path_options;
    path_options.width = component.width;
    path_options.color = mono::Color::OFF_WHITE;
    path_options.uv_mode = mono::UVMode(mono::UVMode::DISTANCE | mono::UVMode::NORMALIZED_WIDTH);
    path_options.closed = false;

    CachedRoad cached_road;
    cached_road.dirty = false;
    cached_road.width = component.width;
    cached_road.texture_name = component.texture_name;
    cached_road.texture = mono::GetTextureFactory()->CreateTexture(component.texture_name.c_str());
    cached_road.buffers = mono::BuildPathDrawBuffers(path->type, path->points, path_options);
    
    return cached_road;
}

bool RoadBatchDrawer::NeedsUpdate(const CachedRoad& road, const RoadComponent& component) const
{
    return
        road.dirty ||
        road.width != component.width ||
        road.texture_name != component.texture_name;
}
