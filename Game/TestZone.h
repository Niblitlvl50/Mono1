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
    struct RemoveEntityEvent;
    struct RemovePhysicsEntityEvent;
    struct ShockwaveEvent;
    
    class TestZone : public mono::PhysicsZone
    {
    public:
        
        TestZone(mono::EventHandler& eventHandler);
        ~TestZone();
                
        virtual void OnLoad(mono::ICameraPtr camera);
        virtual void OnUnload();
        
        void SpawnEntity(const game::SpawnEntityEvent& event);
        void SpawnPhysicsEntity(const game::SpawnPhysicsEntityEvent& event);
        void RemoveEntity(const game::RemoveEntityEvent& event);
        void RemovePhysicsEntity(const game::RemovePhysicsEntityEvent& event);
        void OnShockwaveEvent(const game::ShockwaveEvent& event);

    private:
                
        mono::EventToken<game::SpawnEntityEvent> mSpawnEntityToken;
        mono::EventToken<game::SpawnPhysicsEntityEvent> mSpawnPhysicsEntityToken;
        mono::EventToken<game::RemoveEntityEvent> mRemoveEntityToken;
        mono::EventToken<game::RemovePhysicsEntityEvent> mRemovePhysicsEntityToken;
        mono::EventToken<game::ShockwaveEvent> mShockwaveEventToken;
        
        mono::EventHandler& mEventHandler;
    };
}

