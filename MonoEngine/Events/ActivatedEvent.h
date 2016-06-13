//
//  ActivatedEvent.h
//  Monolith1
//
//  Created by Niblit on 2012-01-07.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//


#pragma once

namespace event
{
    struct ActivatedEvent
    {
        constexpr ActivatedEvent(bool gain)
            : gain(gain)
        { }
        
        const bool gain;
    };
}
