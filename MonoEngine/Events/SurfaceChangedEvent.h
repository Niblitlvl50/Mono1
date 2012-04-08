//
//  SurfaceChangedEvent.h
//  Monolith1
//
//  Created by Niblit on 2012-01-07.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//


#ifndef EVENT_SURFACECHANGEDEVENT_H
#define EVENT_SURFACECHANGEDEVENT_H

namespace Event
{

    struct SurfaceChangedEvent
    {
        SurfaceChangedEvent(int w, int h)
            : width(w),
              height(h)
        { }
    
        const int width;
        const int height;
    };
}

#endif

