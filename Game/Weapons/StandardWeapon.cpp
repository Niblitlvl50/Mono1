//
//  StandardWeapon.cpp
//  MonoOSX
//
//  Created by Niklas Damberg on 19/07/15.
//
//

#include "StandardWeapon.h"
#include "FireBullet.h"
#include "SpawnPhysicsEntityEvent.h"

#include "EventHandler.h"
#include "Vector2f.h"
#include "MathFunctions.h"
#include "AudioFactory.h"
#include "RenderLayers.h"

#include <cmath>

using namespace game;

StandardWeapon::StandardWeapon(mono::EventHandler& eventHandler)
    : BaseWeapon(eventHandler)
{
    m_sound = mono::AudioFactory::CreateSound("laser.wav", false);
}

void StandardWeapon::DoFire(const math::Vector2f& position, float direction) const
{
    const math::Vector2f unit(-std::sin(direction), std::cos(direction));
    const math::Vector2f& new_position = position + (unit * 20.0f);
    const math::Vector2f& impulse = unit * 500.0f;

    auto bullet = std::make_shared<FireBullet>(new_position, direction, m_eventHandler);
    bullet->GetPhysics().body->ApplyImpulse(impulse, new_position);

    m_eventHandler.DispatchEvent(game::SpawnPhysicsEntityEvent(bullet, BACKGROUND));

    m_sound->Position(position.x, position.y);
    m_sound->Play();
}

int StandardWeapon::RoundsPerSecond() const
{
    return 10;
}
