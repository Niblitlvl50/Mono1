//
//  Mono1
//
//  Created by Niblit on 2012-04-11.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#pragma once

namespace event
{
    struct KeyDownEvent
    {
        constexpr KeyDownEvent(int key, bool ctrl, bool shift, bool alt)
            : key(key),
              ctrl(ctrl),
              shift(shift),
              alt(alt)
        { }
        
        const int key;
        const bool ctrl;
        const bool shift;
        const bool alt;
    };

    struct KeyUpEvent
    {
        constexpr KeyUpEvent(int key, bool ctrl, bool shift, bool alt)
            : key(key),
              ctrl(ctrl),
              shift(shift),
              alt(alt)
        { }
        
        const int key;
        const bool ctrl;
        const bool shift;
        const bool alt;
    };
}

