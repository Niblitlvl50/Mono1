//
//  SpawnEntityEvent.h
//  Mono1
//
//  Created by Niblit on 2013-01-27.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#pragma once

#include "MonoFwd.h"

namespace game
{
    struct SpawnEntityEvent
    {
        SpawnEntityEvent(const mono::IEntityPtr entity)
            : mEntity(entity)
        { }
        const mono::IEntityPtr mEntity;
    };
    
    typedef std::function<void (const game::SpawnEntityEvent& event)> SpawnEntityFunc;
}

