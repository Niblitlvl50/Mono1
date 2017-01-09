
#pragma once

#include "IWeaponFactory.h"
#include "MonoFwd.h"

namespace game
{
    class WeaponFactory : public IWeaponFactory
    {
    public:
        WeaponFactory(mono::EventHandler& eventHandler);
        std::unique_ptr<IWeaponSystem> CreateWeapon(WeaponType weapon, WeaponFaction faction) override;

    private:
        mono::EventHandler& m_eventHandler;
    };
}
