
#pragma once

namespace event
{
    enum class TouchType
    {
        DOWN,
        UP,
        MOTION
    };
    
    struct TouchEvent
    {
        constexpr TouchEvent(TouchType type, int64_t touch_id, float x, float y, float dx, float dy)
            : type(type),
              touch_id(touch_id),
              x(x),
              y(y),
              dX(dx),
              dY(dy)
        { }
        
        const TouchType type;
        const int64_t touch_id;
        const float x;
        const float y;
        const float dX;
        const float dY;
    };
}
