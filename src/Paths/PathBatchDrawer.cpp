
#include "PathBatchDrawer.h"
#include "PathSystem.h"
#include "PathFactory.h"

#include "TransformSystem/TransformSystem.h"
#include "Rendering/Color.h"
#include "Rendering/IRenderer.h"
#include "Rendering/RenderBuffer/IRenderBuffer.h"

#include "Math/Quad.h"

using namespace mono;


PathBatchDrawer::PathBatchDrawer(const mono::PathSystem* path_system, const mono::TransformSystem* transform_system)
    : m_path_system(path_system)
    , m_transform_system(transform_system)
    , m_enabled(true)
{ }

void PathBatchDrawer::Draw(mono::IRenderer& renderer) const
{
    if(!m_enabled)
        return;

    struct DrawData
    {
        mono::PathDrawBuffer buffers;
        math::Matrix world_transform;
    };

    std::vector<DrawData> draw_data;

    const auto collect_paths = [this, &draw_data](const mono::PathComponent& component, uint32_t index) {

        if(component.points.empty())
            return;

        draw_data.push_back({
            mono::BuildPathDrawBuffers(component.type, component.points, {1.0f, mono::Color::OFF_WHITE, component.closed}),
            m_transform_system->GetWorld(index),
        });
    };

    m_path_system->ForEach(collect_paths);

    for(const auto& data : draw_data)
    {
        const auto scope = mono::MakeTransformScope(data.world_transform, &renderer);
        renderer.DrawTrianges(data.buffers.vertices.get(), data.buffers.colors.get(), data.buffers.indices.get(), 0, data.buffers.indices->Size());
    }
}

math::Quad PathBatchDrawer::BoundingBox() const
{
    return math::InfQuad;
}
