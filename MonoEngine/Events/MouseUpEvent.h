//
//  MouseUpEvent.h
//  Mono1
//
//  Created by Niblit on 2012-04-11.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//


#pragma once

namespace Event
{
    struct MouseUpEvent
    {
        MouseUpEvent(unsigned int key, int x, int y)
            : mKey(key),
              mX(x),
              mY(y)
        { }
        
        const unsigned int mKey;
        const unsigned int mX;
        const unsigned int mY;
    };
}

