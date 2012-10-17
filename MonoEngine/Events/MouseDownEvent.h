//
//  MouseDownEvent.h
//  Mono1
//
//  Created by Niblit on 2012-10-15.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#pragma once

namespace Event
{
    struct MouseDownEvent
    {
        MouseDownEvent(unsigned int key, int x, int y)
            : mKey(key),
              mX(x),
              mY(y)
        { }
        
        const unsigned int mKey;
        const int mX;
        const int mY;
    };
}

