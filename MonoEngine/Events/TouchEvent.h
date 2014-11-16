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
        TouchEvent(TouchType type, float x, float y, float dx, float dy)
            : mType(type),
              mX(x),
              mY(y),
              mDX(dx),
              mDY(dy)
        { }
        
        const TouchType mType;
        const float mX;
        const float mY;
        const float mDX;
        const float mDY;
    };
}
