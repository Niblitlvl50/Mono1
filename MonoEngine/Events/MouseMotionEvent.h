//
//  MouseMotionEvent.h
//  Mono1
//
//  Created by Niblit on 2012-04-20.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//


#pragma once

namespace Event
{
    struct MouseMotionEvent
    {
        MouseMotionEvent(int x, int y, float worldX, float worldY)
            : screenX(x),
              screenY(y),
              worldX(worldX),
              worldY(worldY)
        { }
        
        const int screenX;
        const int screenY;
        const float worldX;
        const float worldY;
    };
}
