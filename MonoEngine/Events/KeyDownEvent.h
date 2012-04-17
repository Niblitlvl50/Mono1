//
//  KeyDownEvent.h
//  Mono1
//
//  Created by Niblit on 2012-04-11.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#pragma once

namespace Event
{
    struct KeyDownEvent
    {
        KeyDownEvent(unsigned int key)
            : mKey(key)
        { }
        
        const unsigned int mKey;
    };
}

