
#include "RoadBatchDrawer.h"
#include "RoadSystem.h"
#include "Paths/PathSystem.h"
#include "Paths/PathFactory.h"
#include "TransformSystem/TransformSystem.h"

#include "Rendering/IRenderer.h"
#include "Rendering/RenderBuffer/IRenderBuffer.h"

using namespace mono;

RoadBatchDrawer::RoadBatchDrawer(const RoadSystem* road_system, const mono::PathSystem* path_system, const mono::TransformSystem* transform_system)
    : m_road_system(road_system)
    , m_path_system(path_system)
    , m_transform_system(transform_system)
{ }

void RoadBatchDrawer::Draw(mono::IRenderer& renderer) const
{
    const auto collect_roads = [this](uint32_t entity_id, const RoadComponent& component) {
        const math::Matrix& world_transform = m_transform_system->GetWorld(entity_id);
        const mono::PathComponent* path = m_path_system->GetPath(entity_id);

        mono::PathOptions path_options;
        path_options.width = component.width;
        path_options.color = mono::Color::OFF_WHITE;
        path_options.closed = false;

        mono::PathDrawBuffer draw_buffer = mono::BuildPathDrawBuffers(path->type, path->points, path_options);

    };

    m_road_system->ForEeach(collect_roads);
}

math::Quad RoadBatchDrawer::BoundingBox() const
{
    return math::InfQuad;
}
