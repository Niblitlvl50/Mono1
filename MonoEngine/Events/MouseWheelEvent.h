//
//  MouseWheelEvent.h
//  Mono1
//
//  Created by Niblit on 2012-08-27.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//


#pragma once

namespace event
{
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
