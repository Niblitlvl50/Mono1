
#pragma once

#include "System/Keycodes.h"
#include <cstdint>

namespace event
{
    struct MouseDownEvent
    {
        constexpr MouseDownEvent(
            MouseButton key, int screen_x, int screen_y, float world_x, float world_y, bool ctrl, bool shift, bool alt, bool super, uint32_t timestamp)
            : key(key)
            , screen_x(screen_x)
            , screen_y(screen_y)
            , world_x(world_x)
            , world_y(world_y)
            , ctrl(ctrl)
            , shift(shift)
            , alt(alt)
            , super(super)
            , timestamp(timestamp)
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

        const uint32_t timestamp;
    };

    struct MouseUpEvent
    {
        constexpr MouseUpEvent(
            MouseButton key, int screen_x, int screen_y, float world_x, float world_y, bool ctrl, bool shift, bool alt, bool super, uint32_t timestamp)
            : key(key)
            , screen_x(screen_x)
            , screen_y(screen_y)
            , world_x(world_x)
            , world_y(world_y)
            , ctrl(ctrl)
            , shift(shift)
            , alt(alt)
            , super(super)
            , timestamp(timestamp)
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

        const uint32_t timestamp;
    };

    struct MouseMotionEvent
    {
        constexpr MouseMotionEvent(
            int screen_x, int screen_y, float world_x, float world_y, bool ctrl, bool shift, bool alt, bool super, uint32_t timestamp)
            : screen_x(screen_x)
            , screen_y(screen_y)
            , world_x(world_x)
            , world_y(world_y)
            , ctrl(ctrl)
            , shift(shift)
            , alt(alt)
            , super(super)
            , timestamp(timestamp)
        { }
        
        const int screen_x;
        const int screen_y;
        const float world_x;
        const float world_y;

        const bool ctrl;
        const bool shift;
        const bool alt;
        const bool super;

        const uint32_t timestamp;
    };

    struct MouseWheelEvent
    {
        constexpr MouseWheelEvent(int x, int y, bool ctrl, bool shift, bool alt, bool super, uint32_t timestamp)
            : x(x)
            , y(y)
            , ctrl(ctrl)
            , shift(shift)
            , alt(alt)
            , super(super)
            , timestamp(timestamp)
        { }
        
        const int x;
        const int y;

        const bool ctrl;
        const bool shift;
        const bool alt;
        const bool super;

        const uint32_t timestamp;
    };
}
