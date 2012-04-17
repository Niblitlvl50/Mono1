//
//  ActivatedEvent.h
//  Monolith1
//
//  Created by Niblit on 2012-01-07.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//


#pragma once

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
