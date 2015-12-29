//
//  RocketLauncher.cpp
//  MonoOSX
//
//  Created by Niklas Damberg on 19/07/15.
//
//

#include "RocketLauncher.h"
#include "EventHandler.h"

#include "Rocket.h"
#include "SpawnPhysicsEntityEvent.h"
#include "Vector2f.h"
#include "MathFunctions.h"

#include <cmath>

using namespace game;

RocketLauncher::RocketLauncher(mono::EventHandler& eventHandler)
    : BaseWeapon(eventHandler)
{ }

void RocketLauncher::DoFire(const math::Vector2f& position, float direction) const
{
    const math::Vector2f unit(-std::sin(direction), std::cos(direction));
    const math::Vector2f& new_position = position + (unit * 20.0f);
    const math::Vector2f& impulse = unit * 500.0f;

    auto bullet = std::make_shared<Rocket>(new_position, direction, m_eventHandler);
    bullet->GetPhysics().body->ApplyImpulse(impulse, math::zeroVec);

    m_eventHandler.DispatchEvent(game::SpawnPhysicsEntityEvent(bullet));
}

int RocketLauncher::RoundsPerSecond() const
{
    return 1;
}
