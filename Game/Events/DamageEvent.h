//
//  DamageEvent.h
//  MonoiOS
//
//  Created by Niklas Damberg on 11/06/16.
//
//

#pragma once

#include "CMFwd.h"

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