//
//  PauseEvent.h
//  Mono1
//
//  Created by Niblit on 2013-03-17.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#pragma once

namespace event
{
    struct PauseEvent
    {
        constexpr PauseEvent(bool pause)
            : pause(pause)
        { }

        const bool pause;
    };
}
