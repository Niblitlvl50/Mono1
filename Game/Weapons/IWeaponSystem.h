//
//  IWeaponSystem.h
//  MonoOSX
//
//  Created by Niklas Damberg on 19/07/15.
//
//

#pragma once

#include "Math/MathFwd.h"

namespace game
{
    class IWeaponSystem
    {
    public:

        virtual ~IWeaponSystem()
        { }

        virtual void Fire(const math::Vector& position, float direction) = 0;
    };
}
