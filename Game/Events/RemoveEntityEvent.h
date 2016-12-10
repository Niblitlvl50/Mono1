//
//  RemoveEntityEvent.h
//  Mono1
//
//  Created by Niblit on 2013-03-21.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#pragma once

#include <functional>

namespace game
{
    struct RemoveEntityEvent
    {
        constexpr RemoveEntityEvent(uint id)
            : id(id)
        { }

        const uint id;
    };
    
    using RemoveEntityFunc = std::function<bool (const game::RemoveEntityEvent& event)>;
}
