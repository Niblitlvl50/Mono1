//
//  TestZone.h
//  Mono1
//
//  Created by Niblit on 2012-04-06.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//


#pragma once

#include "PhysicsZone.h"
#include "EventHandler/EventToken.h"

#include "Spawner.h"
#include "DamageController.h"

namespace game
{
    struct SpawnEntityEvent;
    struct SpawnPhysicsEntityEvent;
    struct RemoveEntityEvent;
    struct ShockwaveEvent;
    struct DamageEvent;
    
    class TestZone : public mono::PhysicsZone
    {
    public:
        
        TestZone(mono::EventHandler& eventHandler);
        ~TestZone();
                
        virtual void OnLoad(mono::ICameraPtr camera);
        virtual void OnUnload();
        
        bool SpawnEntity(const game::SpawnEntityEvent& event);
        bool SpawnPhysicsEntity(const game::SpawnPhysicsEntityEvent& event);
        bool OnRemoveEntity(const game::RemoveEntityEvent& event);
        bool OnShockwaveEvent(const game::ShockwaveEvent& event);
        bool OnDamageEvent(const game::DamageEvent& event);

    private:

        virtual void AddPhysicsEntity(const mono::IPhysicsEntityPtr& entity, int layer);
        virtual void RemovePhysicsEntity(const mono::IPhysicsEntityPtr& entity);
        virtual void AddEntity(const mono::IEntityPtr& entity, int layer);
        virtual void RemoveEntity(const mono::IEntityPtr& entity);


        mono::EventToken<game::SpawnEntityEvent> mSpawnEntityToken;
        mono::EventToken<game::SpawnPhysicsEntityEvent> mSpawnPhysicsEntityToken;
        mono::EventToken<game::RemoveEntityEvent> mRemoveEntityByIdToken;
        mono::EventToken<game::ShockwaveEvent> mShockwaveEventToken;
        mono::EventToken<game::DamageEvent> mDamageEventToken;
        
        mono::EventHandler& mEventHandler;
        Spawner m_spawner;
        DamageController m_damageController;
        
        mono::ISoundPtr m_backgroundMusic;
    };
}

