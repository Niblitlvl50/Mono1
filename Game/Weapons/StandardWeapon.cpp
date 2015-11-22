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

#include <cmath>

using namespace game;

StandardWeapon::StandardWeapon(mono::EventHandler& eventHandler)
    : mEventHandler(eventHandler)
{ }

void StandardWeapon::Fire(const math::Vector2f& position, float direction)
{
    const math::Vector2f unit(-std::sin(math::ToRadians(direction)),
                               std::cos(math::ToRadians(direction)));
    const math::Vector2f& new_position = position + (unit * 20.0f);
    const math::Vector2f& impulse = unit * 500.0f;

    auto bullet = std::make_shared<FireBullet>(new_position, direction, mEventHandler);
    bullet->GetPhysics().body->ApplyImpulse(impulse, math::zeroVec);

    mEventHandler.DispatchEvent(game::SpawnPhysicsEntityEvent(bullet));
}

int StandardWeapon::RoundsPerSecond() const
{
    return 10;
}

const char* StandardWeapon::SpriteFile() const
{
    return "StandardWeaponSprite.sprite";
}
