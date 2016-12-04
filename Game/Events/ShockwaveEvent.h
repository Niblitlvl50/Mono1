//
//  ShockwaveEvent.h
//  MonoOSX
//
//  Created by Niklas Damberg on 16/11/14.
//
//

#pragma once

#include "MonoFwd.h"
#include "Math/Vector2f.h"

namespace game
{
    struct ShockwaveEvent
    {
        ShockwaveEvent(const math::Vector2f& position, float magnitude)
            : position(position),
              magnitude(magnitude)
        { }

        const math::Vector2f position;
        const float magnitude;
    };

    typedef std::function<bool (const game::ShockwaveEvent&)> ShockwaveEventFunc;
}
