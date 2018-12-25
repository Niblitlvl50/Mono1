
#pragma once

namespace event
{
    struct MultiGestureEvent
    {
        constexpr MultiGestureEvent(float x, float y, float theta, float distance)
            : x(x),
              y(y),
              theta(theta),
              distance(distance)
        { }
        
        const float x;
        const float y;
        const float theta;
        const float distance;
    };
}
