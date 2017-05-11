
#pragma once

#include "System/Keycodes.h"

namespace event
{
    struct MouseDownEvent
    {
        constexpr MouseDownEvent(MouseButton key, int screenX, int screenY, float worldX, float worldY)
            : key(key),
              screenX(screenX),
              screenY(screenY),
              worldX(worldX),
              worldY(worldY)
        { }
        
        const MouseButton key;
        const int screenX;
        const int screenY;
        const float worldX;
        const float worldY;
    };

    struct MouseUpEvent
    {
        constexpr MouseUpEvent(MouseButton key, int screenX, int screenY, float worldX, float worldY)
            : key(key),
              screenX(screenX),
              screenY(screenY),
              worldX(worldX),
              worldY(worldY)
        { }
        
        const MouseButton key;
        const int screenX;
        const int screenY;
        const float worldX;
        const float worldY;
    };

    struct MouseMotionEvent
    {
        constexpr MouseMotionEvent(int x, int y, float worldX, float worldY)
            : screenX(x),
              screenY(y),
              worldX(worldX),
              worldY(worldY)
        { }
        
        const int screenX;
        const int screenY;
        const float worldX;
        const float worldY;
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
