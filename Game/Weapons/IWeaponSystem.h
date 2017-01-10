
#pragma once

#include "Math/MathFwd.h"

namespace game
{
    class IWeaponSystem
    {
    public:

        virtual ~IWeaponSystem()
        { }

        virtual bool Fire(const math::Vector& position, float direction) = 0;
    };
}
