//
//  MultiGestureEvent.h
//  MonoiOS
//
//  Created by Niblit on 2012-09-17.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#pragma once

namespace Event
{
    struct MultiGestureEvent
    {
        MultiGestureEvent(int x, int y, float theta, float distance)
        : mX(x),
          mY(y),
          mTheta(theta),
          mDistance(distance)
        { }
        
        const int mX;
        const int mY;
        const float mTheta;
        const float mDistance;
    };
}
