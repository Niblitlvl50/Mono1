//
//  KeyUpEvent.h
//  Mono1
//
//  Created by Niblit on 2012-04-11.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//


#pragma once

namespace Event
{
    struct KeyUpEvent
    {
        constexpr KeyUpEvent(int key)
            : key(key)
        { }
        
        const int key;
    };
}

