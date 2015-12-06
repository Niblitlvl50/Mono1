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
