
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
        virtual void Fire(const math::Vector2f& position, float direction);

    private:

        const WeaponConfiguration m_weaponConfig;
        mono::EventHandler& m_eventHandler;
        unsigned int m_lastFireTimestamp;
        float m_currentFireRate;

        mono::ISoundPtr m_fireSound;
    };
}
