//
//  SurfaceChangedEvent.h
//  Monolith1
//
//  Created by Niblit on 2012-01-07.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//


#pragma once

namespace event
{
    struct SurfaceChangedEvent
    {
        constexpr SurfaceChangedEvent(int w, int h)
            : width(w),
              height(h)
        { }
        
        const int width;
        const int height;
    };
}

