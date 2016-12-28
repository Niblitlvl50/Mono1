
#pragma once

#include "MonoFwd.h"
#include "Math/Vector.h"

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
}
