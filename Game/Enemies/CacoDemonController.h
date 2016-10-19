
#pragma once

#include "Weapons/IWeaponSystem.h"
#include "Enemy.h"

namespace game
{
    class CacoDemonController : public IEnemyController
    {
    public:

        CacoDemonController(mono::EventHandler& event_handler);

        virtual void Initialize(Enemy* enemy);
        virtual void doUpdate(unsigned int delta);

    private:

        std::unique_ptr<IWeaponSystem> m_weapon;
        Enemy* m_enemy;
    };
}
