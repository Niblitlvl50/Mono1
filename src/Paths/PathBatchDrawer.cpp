
#include "PathBatchDrawer.h"
#include "PathSystem.h"
#include "TransformSystem/TransformSystem.h"

#include "Rendering/Color.h"
#include "Rendering/IRenderer.h"

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

    const auto collect_paths = [this, &renderer](const mono::PathComponent& component, uint32_t index) {
        const math::Matrix& transform = m_transform_system->GetWorld(index);
        const auto scope = mono::MakeTransformScope(transform, &renderer);
        renderer.DrawPolyline(component.points, mono::Color::RED, 1.0f);
    };

    m_path_system->ForEach(collect_paths);
}

math::Quad PathBatchDrawer::BoundingBox() const
{
    return math::InfQuad;
}
