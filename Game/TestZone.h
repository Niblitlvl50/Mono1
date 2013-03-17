//
//  TestZone.h
//  Mono1
//
//  Created by Niblit on 2012-04-06.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//


#pragma once

#include "PhysicsZone.h"
#include "EventToken.h"

namespace game
{
    struct SpawnEntityEvent;
    struct SpawnPhysicsEntityEvent;
    
    class TestZone : public mono::PhysicsZone
    {
    public:
        
        TestZone();
        ~TestZone();
                
        virtual void OnLoad(mono::ICameraPtr camera);
        virtual void OnUnload();
        
        void SpawnEntity(const game::SpawnEntityEvent& event);
        void SpawnPhysicsEntity(const game::SpawnPhysicsEntityEvent& event);

    private:
        
        mono::EventToken<game::SpawnEntityEvent> mSpawnEntityToken;
        mono::EventToken<game::SpawnPhysicsEntityEvent> mSpawnPhysicsEntityToken;
    };
}

