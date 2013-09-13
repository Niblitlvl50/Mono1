//
//  SpawnPhysicsEntityEvent.h
//  Mono1
//
//  Created by Niblit on 2013-01-27.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#pragma once

#include "MonoFwd.h"

namespace game
{
    struct SpawnPhysicsEntityEvent
    {
        SpawnPhysicsEntityEvent(mono::IPhysicsEntityPtr entity)
            : mEntity(entity)
        { }
        mono::IPhysicsEntityPtr mEntity;
    };
    
    typedef std::function<void (const game::SpawnPhysicsEntityEvent& event)> SpawnPhysicsEntityFunc;
}
