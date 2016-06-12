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
        DamageEvent(const cm::IBodyPtr& body, int damage)
            : body(body),
              damage(damage)
        { }

        const cm::IBodyPtr body;
        const int damage;
    };
}