//
//  ActivatedEvent.h
//  Monolith1
//
//  Created by Niblit on 2012-01-07.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//


#ifndef EVENT_ACTIVATEDEVENT_H
#define EVENT_ACTIVATEDEVENT_H

namespace Event
{
    struct ActivatedEvent
    {
        ActivatedEvent(bool gain)
            : gain(gain)
        { }
        
        const bool gain;
    };
}

#endif

