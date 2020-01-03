
#include "TransformSystemDrawer.h"
#include "TransformSystem.h"
#include "Math/Quad.h"
#include "Rendering/IRenderer.h"
#include "Rendering/Color.h"

#include <cstdio>

using namespace mono;

TransformSystemDrawer::TransformSystemDrawer(const TransformSystem* transform_system)
    : m_transform_system(transform_system)
{ }

void TransformSystemDrawer::doDraw(mono::IRenderer& renderer) const
{
    const auto draw_bounding_boxes = [this, &renderer](const TransformSystem::Component& component, uint32_t index) {
        const math::Quad& bb = m_transform_system->GetWorldBoundingBox(index);
        if(!renderer.Cull(bb))
            return;

        const math::Matrix& world_transform = m_transform_system->GetWorld(index);
        char buffer[8] = { 0 };
        std::sprintf(buffer, "%u", index);

        renderer.DrawQuad(bb, mono::Color::RED, 1.0f);
        renderer.DrawText(0, buffer, math::GetPosition(world_transform), true, mono::Color::RED);
    };

    m_transform_system->ForEachComponent(draw_bounding_boxes);
}

math::Quad TransformSystemDrawer::BoundingBox() const
{
    return math::InfQuad;
}