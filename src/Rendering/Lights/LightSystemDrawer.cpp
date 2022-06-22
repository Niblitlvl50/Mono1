
#include "LightSystemDrawer.h"
#include "LightSystem.h"
#include "TransformSystem/TransformSystem.h"

#include "Math/Matrix.h"
#include "Math/Quad.h"
#include "Rendering/IRenderer.h"

#include "Util/Noise.h"

using namespace mono;

LightSystemDrawer::LightSystemDrawer(const LightSystem* light_system, const TransformSystem* transform_system)
    : m_light_system(light_system)
    , m_transform_system(transform_system)
{ }

void LightSystemDrawer::Draw(mono::IRenderer& renderer) const
{
    const auto register_lights = [this, &renderer](const LightComponent& light, uint32_t entity_id) {
        const math::Matrix& world_transform = m_transform_system->GetWorld(entity_id);
        const math::Vector world_position = math::GetPosition(world_transform) + light.offset;

        const math::Quad light_bb = math::Quad(world_position, light.radius);
        if(renderer.Cull(light_bb))
        {
            float radius_flicker = 0.0f;

            if(light.flicker)
            {
                auto pair = m_flicker_data.insert(std::make_pair(entity_id, 0.0f)); // Initialize with zero, or get what's in there already.
                float& counter = pair.first->second;
                counter += renderer.GetDeltaTime() * light.flicker_frequencey;

                const float noise = mono::Noise::Perlin(world_position.x + counter, world_position.y);
                radius_flicker = light.radius * noise * light.flicker_percentage;
            }

            renderer.AddLight(world_position, light.radius + radius_flicker, light.shade);
        }
    };

    m_light_system->ForEach(register_lights);
}

math::Quad LightSystemDrawer::BoundingBox() const
{
    return math::InfQuad;
}
