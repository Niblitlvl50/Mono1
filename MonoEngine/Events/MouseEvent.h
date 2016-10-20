
#pragma once

namespace event
{
    struct MouseDownEvent
    {
        constexpr MouseDownEvent(unsigned int key, int screenX, int screenY, float worldX, float worldY)
            : key(key),
              screenX(screenX),
              screenY(screenY),
              worldX(worldX),
              worldY(worldY)
        { }
        
        const unsigned int key;
        const int screenX;
        const int screenY;
        const float worldX;
        const float worldY;
    };

    struct MouseUpEvent
    {
        constexpr MouseUpEvent(unsigned int key, int screenX, int screenY, float worldX, float worldY)
            : key(key),
              screenX(screenX),
              screenY(screenY),
              worldX(worldX),
              worldY(worldY)
        { }
        
        const unsigned int key;
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
