//
//  SpawnPhysicsEntityEvent.h
//  Mono1
//
//  Created by Niblit on 2013-01-27.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#pragma once

#include "MonoFwd.h"
#include "CMFwd.h"
#include <tr1/functional>

namespace game
{
    struct SpawnPhysicsEntityEvent
    {
        SpawnPhysicsEntityEvent(mono::IEntityPtr entity, cm::Object& physics)
            : mEntity(entity),
              mPhysics(physics)
        { }
        
        mono::IEntityPtr mEntity;
        cm::Object& mPhysics;
    };
    
    typedef std::tr1::function<void (const game::SpawnPhysicsEntityEvent& event)> SpawnPhysicsEntityFunc;
}
