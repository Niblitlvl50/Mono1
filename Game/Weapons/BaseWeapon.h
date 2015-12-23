//
//  BaseWeapon.hpp
//  MonoOSX
//
//  Created by Niklas Damberg on 23/12/15.
//
//

#pragma once

#include "IWeaponSystem.h"
#include "MonoFwd.h"

namespace game
{
    class BaseWeapon : public IWeaponSystem
    {
    public:

        BaseWeapon(mono::EventHandler& eventHandler);
        virtual void Fire(const math::Vector2f& position, float direction);

    protected:

        virtual void DoFire(const math::Vector2f& position, float direction) const = 0;

        mono::EventHandler& m_eventHandler;
        unsigned int m_lastFireTimestamp;
    };
}
