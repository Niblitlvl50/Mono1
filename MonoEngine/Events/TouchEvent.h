//
//  TouchEvent.h
//  MonoiOS
//
//  Created by Niklas Damberg on 9/30/13.
//
//

#pragma once

namespace Event
{
    enum class TouchType
    {
        DOWN,
        UP,
        MOTION
    };
    
    struct TouchEvent
    {
        TouchEvent(TouchType type, int64_t touchId, float x, float y, float dx, float dy)
            : type(type),
              touchId(touchId),
              x(x),
              y(y),
              dX(dx),
              dY(dy)
        { }
        
        const TouchType type;
        const int64_t touchId;
        const float x;
        const float y;
        const float dX;
        const float dY;
    };
}
