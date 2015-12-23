//
//  BaseWeapon.cpp
//  MonoOSX
//
//  Created by Niklas Damberg on 23/12/15.
//
//

#include "BaseWeapon.h"
#include "SysTime.h"

using namespace game;

BaseWeapon::BaseWeapon(mono::EventHandler& eventHandler)
    : m_eventHandler(eventHandler),
      m_lastFireTimestamp(0)
{ }

void BaseWeapon::Fire(const math::Vector2f& position, float direction)
{
    const float rpsHz = 1.0 / RoundsPerSecond();
    const unsigned int weaponDelta = rpsHz * 1000;

    const unsigned int now = Time::GetMilliseconds();
    const unsigned int delta = now - m_lastFireTimestamp;

    if(delta > weaponDelta)
    {
        DoFire(position, direction);
        m_lastFireTimestamp = now;
    }

}
