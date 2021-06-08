
#include "LightSystemDrawer.h"
#include "LightSystem.h"
#include "TransformSystem/TransformSystem.h"

#include "Math/Matrix.h"
#include "Math/Quad.h"
#include "Rendering/IRenderer.h"

using namespace mono;

LightSystemDrawer::LightSystemDrawer(const LightSystem* light_system, const TransformSystem* transform_system)
    : m_light_system(light_system)
    , m_transform_system(transform_system)
{ }

void LightSystemDrawer::Draw(mono::IRenderer& renderer) const
{
    const auto register_lights = [this, &renderer](const LightComponent& light, uint32_t entity_id) {
        const math::Matrix& world_transform = m_transform_system->GetWorld(entity_id);
        const math::Vector world_position = math::GetPosition(world_transform);

        const math::Quad light_bb = math::Quad(world_position, light.radius);
        if(renderer.Cull(light_bb))
            renderer.AddLight(world_position, light.radius, light.shade);
    };

    m_light_system->ForEach(register_lights);
}

math::Quad LightSystemDrawer::BoundingBox() const
{
    return math::InfQuad;
}
