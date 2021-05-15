
#pragma once

#include "System/Keycodes.h"

namespace event
{
    struct MouseDownEvent
    {
        constexpr MouseDownEvent(MouseButton key, int screen_x, int screen_y, float world_x, float world_y, bool ctrl, bool shift, bool alt, bool super)
            : key(key)
            , screen_x(screen_x)
            , screen_y(screen_y)
            , world_x(world_x)
            , world_y(world_y)
            , ctrl(ctrl)
            , shift(shift)
            , alt(alt)
            , super(super)
        { }
        
        const MouseButton key;
        const int screen_x;
        const int screen_y;
        const float world_x;
        const float world_y;

        const bool ctrl;
        const bool shift;
        const bool alt;
        const bool super;
    };

    struct MouseUpEvent
    {
        constexpr MouseUpEvent(MouseButton key, int screen_x, int screen_y, float world_x, float world_y, bool ctrl, bool shift, bool alt, bool super)
            : key(key)
            , screen_x(screen_x)
            , screen_y(screen_y)
            , world_x(world_x)
            , world_y(world_y)
            , ctrl(ctrl)
            , shift(shift)
            , alt(alt)
            , super(super)
        { }
        
        const MouseButton key;
        const int screen_x;
        const int screen_y;
        const float world_x;
        const float world_y;

        const bool ctrl;
        const bool shift;
        const bool alt;
        const bool super;
    };

    struct MouseMotionEvent
    {
        constexpr MouseMotionEvent(int screen_x, int screen_y, float world_x, float world_y, bool ctrl, bool shift, bool alt, bool super)
            : screen_x(screen_x)
            , screen_y(screen_y)
            , world_x(world_x)
            , world_y(world_y)
            , ctrl(ctrl)
            , shift(shift)
            , alt(alt)
            , super(super)
        { }
        
        const int screen_x;
        const int screen_y;
        const float world_x;
        const float world_y;

        const bool ctrl;
        const bool shift;
        const bool alt;
        const bool super;
    };

    struct MouseWheelEvent
    {
        constexpr MouseWheelEvent(int x, int y, bool ctrl, bool shift, bool alt, bool super)
            : x(x)
            , y(y)
            , ctrl(ctrl)
            , shift(shift)
            , alt(alt)
            , super(super)
        { }
        
        const int x;
        const int y;

        const bool ctrl;
        const bool shift;
        const bool alt;
        const bool super;
    };
}
