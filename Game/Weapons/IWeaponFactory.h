
#pragma once

#include "WeaponTypes.h"
#include <memory>

namespace game
{
    class IWeaponSystem;

    class IWeaponFactory
    {
    public:
        virtual ~IWeaponFactory()
        { }

        virtual std::unique_ptr<IWeaponSystem> CreateWeapon(WeaponType weapon, WeaponFaction faction) = 0;
    };
}
