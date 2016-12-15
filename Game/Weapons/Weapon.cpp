
#include "Weapon.h"
#include "Bullet.h"

#include "Math/Vector.h"
#include "System/SysTime.h"
#include "EventHandler/EventHandler.h"
#include "Audio/ISound.h"
#include "Audio/AudioFactory.h"

#include "Events/SpawnPhysicsEntityEvent.h"
#include "RenderLayers.h"

#include <cmath>

using namespace game;

Weapon::Weapon(const WeaponConfiguration& config, mono::EventHandler& eventHandler)
    : m_weaponConfig(config),
      m_eventHandler(eventHandler),
      m_lastFireTimestamp(0),
      m_currentFireRate(1.0f)
{
    if(config.fire_sound)
        m_fireSound = mono::AudioFactory::CreateSound(config.fire_sound, false, false);
}

void Weapon::Fire(const math::Vector& position, float direction)
{
    const float rpsHz = 1.0f / m_weaponConfig.rounds_per_second;
    const unsigned int weapon_delta = rpsHz * 1000.0f;

    const unsigned int now = Time::GetMilliseconds();
    const unsigned int delta = now - m_lastFireTimestamp;
    const unsigned int modified_delta = delta * m_currentFireRate;

    if(delta > weapon_delta)
        m_currentFireRate = 1.0f;

    if(modified_delta > weapon_delta)
    {
        const math::Vector unit(-std::sin(direction), std::cos(direction));
        const math::Vector& impulse = unit * m_weaponConfig.bullet_force;

        auto bullet = std::make_shared<Bullet>(m_weaponConfig.bullet_config, m_eventHandler);
        bullet->SetPosition(position);
        bullet->SetRotation(direction);
        bullet->GetPhysics().body->ApplyImpulse(impulse, position);

        m_eventHandler.DispatchEvent(game::SpawnPhysicsEntityEvent(bullet, BACKGROUND));

        if(m_fireSound)
            m_fireSound->Play();

        m_lastFireTimestamp = now;
        m_currentFireRate *= m_weaponConfig.fire_rate_multiplier;
        m_currentFireRate = std::min(m_currentFireRate, m_weaponConfig.max_fire_rate);
    }
}
