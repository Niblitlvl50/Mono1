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
        constexpr MouseUpEvent(unsigned int key, int screenX, int screenY, float worldX, float worldY)
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

