//
//  EventFwd.h
//  Monolith1
//
//  Created by Niblit on 2012-01-07.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#ifndef EVENT_EVENTFWD_H
#define EVENT_EVENTFWD_H

#include <tr1/functional>

namespace Event
{
    struct QuitEvent;
    typedef std::tr1::function<void (const QuitEvent&)> QuitEventFunc;
    
    struct SurfaceChangedEvent;
    typedef std::tr1::function<void (const SurfaceChangedEvent&)> SurfaceChangedEventFunc;
    
    struct ActivatedEvent;
    typedef std::tr1::function<void (const ActivatedEvent&)> ActivatedEventFunc;
}

#endif

