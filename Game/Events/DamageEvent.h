
#pragma once

#include "Physics/CMFwd.h"

namespace game
{
    struct DamageEvent
    {
        DamageEvent(const mono::IBodyPtr& body, int damage)
            : body(body),
              damage(damage)
        { }

        const mono::IBodyPtr body;
        const int damage;
    };
}