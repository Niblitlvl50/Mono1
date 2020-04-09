
#pragma once

#include "System/Keycodes.h"

namespace event
{
    struct MouseDownEvent
    {
        constexpr MouseDownEvent(MouseButton key, int screen_x, int screen_y, float world_x, float world_y)
            : key(key),
              screen_x(screen_x),
              screen_y(screen_y),
              world_x(world_x),
              world_y(world_y)
        { }
        
        const MouseButton key;
        const int screen_x;
        const int screen_y;
        const float world_x;
        const float world_y;
    };

    struct MouseUpEvent
    {
        constexpr MouseUpEvent(MouseButton key, int screen_x, int screen_y, float world_x, float world_y)
            : key(key),
              screen_x(screen_x),
              screen_y(screen_y),
              world_x(world_x),
              world_y(world_y)
        { }
        
        const MouseButton key;
        const int screen_x;
        const int screen_y;
        const float world_x;
        const float world_y;
    };

    struct MouseMotionEvent
    {
        constexpr MouseMotionEvent(int screen_x, int screen_y, float world_x, float world_y)
            : screen_x(screen_x),
              screen_y(screen_y),
              world_x(world_x),
              world_y(world_y)
        { }
        
        const int screen_x;
        const int screen_y;
        const float world_x;
        const float world_y;
    };

    struct MouseWheelEvent
    {
        constexpr MouseWheelEvent(int x, int y)
            : x(x),
              y(y)
        { }
        
        const int x;
        const int y;
    };
}
