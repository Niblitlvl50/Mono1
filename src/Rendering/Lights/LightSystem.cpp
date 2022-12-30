
#include "LightSystem.h"
#include "System/Hash.h"

using namespace mono;

LightSystem::LightSystem(uint32_t n_lights)
{
    m_alive.resize(n_lights, false);
    m_enabled.resize(n_lights, true);
    m_lights.resize(n_lights);
}

void LightSystem::Allocate(uint32_t light_id)
{
    m_alive[light_id] = true;

    LightComponent& light = m_lights[light_id];
    light.radius = 1.0f;
    light.offset = math::ZeroVec;
    light.shade = mono::Color::WHITE;
    light.flicker = false;
    light.flicker_frequencey = 1.0f;
    light.flicker_percentage = 0.5f;
}

bool LightSystem::IsAllocated(uint32_t light_id)
{
    return m_alive[light_id];
}

void LightSystem::SetData(uint32_t light_id, const LightComponent& component_data)
{
    m_lights[light_id] = component_data;
}

void LightSystem::Release(uint32_t light_id)
{
    m_alive[light_id] = false;
}

void LightSystem::SetLightEnabled(uint32_t light_id, bool enabled)
{
    m_enabled[light_id] = enabled;
}

const char* LightSystem::Name() const
{
    return "lightsystem";
}

void LightSystem::Update(const mono::UpdateContext& update_context)
{ }
