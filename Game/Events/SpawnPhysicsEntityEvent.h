//
//  SpawnPhysicsEntityEvent.h
//  Mono1
//
//  Created by Niblit on 2013-01-27.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#pragma once

#include "MonoFwd.h"
#include "MonoPtrFwd.h"

namespace game
{
    struct SpawnPhysicsEntityEvent
    {
        SpawnPhysicsEntityEvent(const mono::IPhysicsEntityPtr& entity, int layer)
            : mEntity(entity),
              layer(layer)
        { }
        mono::IPhysicsEntityPtr mEntity;
        const int layer;
    };
    
    typedef std::function<bool (const game::SpawnPhysicsEntityEvent& event)> SpawnPhysicsEntityFunc;
}
