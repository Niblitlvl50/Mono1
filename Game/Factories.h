
#pragma once

namespace game
{
    class IWeaponFactory;
    class IEnemyFactory;

    extern IWeaponFactory* weapon_factory;
    extern IEnemyFactory* enemy_factory;
}
