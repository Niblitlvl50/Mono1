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
        MouseUpEvent(unsigned int key, int screenX, int screenY, float worldX, float worldY)
            : mKey(key),
              screenX(screenX),
              screenY(screenY),
              worldX(worldX),
              worldY(worldY)
        { }
        
        const unsigned int mKey;
        const int screenX;
        const int screenY;
        const float worldX;
        const float worldY;
    };
}

