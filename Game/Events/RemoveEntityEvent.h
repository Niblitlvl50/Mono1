//
//  RemoveEntityEvent.h
//  Mono1
//
//  Created by Niblit on 2013-03-21.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#pragma once

#include "MonoFwd.h"

namespace game
{
    struct RemoveEntityEvent
    {
        RemoveEntityEvent(const mono::IEntityPtr entity)
            : mEntity(entity)
        { }
        const mono::IEntityPtr mEntity;
    };

    struct RemoveEntityByIdEvent
    {
        constexpr RemoveEntityByIdEvent(uint id)
            : id(id)
        { }

        const uint id;
    };
    
    typedef std::function<void (const game::RemoveEntityEvent& event)> RemoveEntityFunc;
    typedef std::function<void (const game::RemoveEntityByIdEvent& event)> RemoveEntityByIdFunc;
}
