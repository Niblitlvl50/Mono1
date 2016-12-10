//
//  SpawnEntityEvent.h
//  Mono1
//
//  Created by Niblit on 2013-01-27.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#pragma once

#include "MonoFwd.h"
#include <functional>

namespace game
{
    struct SpawnEntityEvent
    {
        SpawnEntityEvent(const mono::IEntityPtr& entity)
            : entity(entity)
        { }
        const mono::IEntityPtr entity;
    };
    
    using SpawnEntityFunc = std::function<bool (const game::SpawnEntityEvent& event)>;
}

