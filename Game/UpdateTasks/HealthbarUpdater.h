
#pragma once

#include "IUpdatable.h"
#include "MonoFwd.h"
#include <vector>

namespace game
{
    class DamageController;
    struct Healthbar;

    class HealthbarUpdater : public mono::IUpdatable
    {
    public:

        HealthbarUpdater(
            std::vector<Healthbar>& healthbars,
            const DamageController& damage_controller,
            const mono::IPhysicsZone& zone);

        virtual void doUpdate(unsigned int delta);

    private:

        std::vector<Healthbar>& m_healthbars;
        const DamageController& m_damageController;
        const mono::IPhysicsZone& m_zone;
    };
}
