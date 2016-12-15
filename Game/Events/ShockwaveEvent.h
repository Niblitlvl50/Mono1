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

#include <functional>

namespace game
{
    struct ShockwaveEvent
    {
        ShockwaveEvent(const math::Vector& position, float magnitude)
            : position(position),
              magnitude(magnitude)
        { }

        const math::Vector position;
        const float magnitude;
    };

    using ShockwaveEventFunc = std::function<bool (const game::ShockwaveEvent&)>;
}
