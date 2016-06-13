//
//  MouseDownEvent.h
//  Mono1
//
//  Created by Niblit on 2012-10-15.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#pragma once

namespace event
{
    struct MouseDownEvent
    {
        constexpr MouseDownEvent(unsigned int key, int screenX, int screenY, float worldX, float worldY)
            : key(key),
              screenX(screenX),
              screenY(screenY),
              worldX(worldX),
              worldY(worldY)
        { }
        
        const unsigned int key;
        const int screenX;
        const int screenY;
        const float worldX;
        const float worldY;
    };
}

