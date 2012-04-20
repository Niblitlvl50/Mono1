//
//  MouseMotionEvent.h
//  Mono1
//
//  Created by Niblit on 2012-04-20.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//


#pragma once

namespace Event
{
    struct MouseMotionEvent
    {
        MouseMotionEvent(int x, int y)
            : mX(x),
              mY(y)
        { }
        
        const int mX;
        const int mY;
    };
}
