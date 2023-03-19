
#include "TransformSystemDrawer.h"
#include "TransformSystem.h"
#include "Math/Quad.h"
#include "Rendering/IRenderer.h"
#include "Rendering/Color.h"

#include <cstdio>

using namespace mono;

TransformSystemDrawer::TransformSystemDrawer(const bool& enabled, const TransformSystem* transform_system)
    : m_enabled(enabled)
    , m_transform_system(transform_system)
{ }

void TransformSystemDrawer::Draw(mono::IRenderer& renderer) const
{
    if(!m_enabled)
        return;

    const auto draw_bounding_boxes = [this, &renderer](const TransformSystem::Component& component, uint32_t index) {
        const math::Quad& bb = m_transform_system->GetWorldBoundingBox(index);
        if(!renderer.Cull(bb))
            return;

        renderer.DrawQuad(bb, mono::Color::RED, 1.0f);

        const math::Matrix& world_transform = m_transform_system->GetWorld(index);
        const auto scope = mono::MakeTransformScope(world_transform, &renderer);
        
        char buffer[8] = { 0 };
        std::snprintf(buffer, std::size(buffer), "%u", index);
        renderer.RenderText(0, buffer, mono::Color::RED, mono::FontCentering::HORIZONTAL_VERTICAL);
    };

    m_transform_system->ForEachComponent(draw_bounding_boxes);
}

math::Quad TransformSystemDrawer::BoundingBox() const
{
    return math::InfQuad;
}
