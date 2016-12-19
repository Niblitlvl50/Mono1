
#pragma once

#include "IWeaponSystem.h"
#include "WeaponConfiguration.h"

#include "MonoPtrFwd.h"
#include "Math/MathFwd.h"

namespace game
{
    class Weapon : public IWeaponSystem
    {
    public:

        Weapon(const WeaponConfiguration& config, mono::EventHandler& eventHandler);
        virtual bool Fire(const math::Vector& position, float direction);

    private:

        const WeaponConfiguration m_weaponConfig;
        mono::EventHandler& m_eventHandler;
        unsigned int m_lastFireTimestamp;
        float m_currentFireRate;

        mono::ISoundPtr m_fireSound;
    };
}
