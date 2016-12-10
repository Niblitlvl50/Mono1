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
#include <functional>

namespace game
{
    struct SpawnPhysicsEntityEvent
    {
        SpawnPhysicsEntityEvent(const mono::IPhysicsEntityPtr& entity, int layer)
            : entity(entity),
              layer(layer)
        { }
        mono::IPhysicsEntityPtr entity;
        const int layer;
    };
    
    using SpawnPhysicsEntityFunc = std::function<bool (const game::SpawnPhysicsEntityEvent& event)>;
}
