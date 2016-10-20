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
        constexpr KeyDownEvent(int key, int modifier)
            : key(key),
              modifier(modifier)
        { }
        
        const int key;
        const int modifier;
    };

    struct KeyUpEvent
    {
        constexpr KeyUpEvent(int key, int modifier)
            : key(key),
              modifier(modifier)
        { }
        
        const int key;
        const int modifier;
    };
}

