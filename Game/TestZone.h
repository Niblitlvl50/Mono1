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

#include "Spawner.h"
#include "DamageController.h"

#include "SysNetwork.h"

namespace game
{
    struct SpawnEntityEvent;
    struct SpawnPhysicsEntityEvent;
    struct RemoveEntityByIdEvent;
    struct ShockwaveEvent;
    struct DamageEvent;
    
    class TestZone : public mono::PhysicsZone
    {
    public:
        
        TestZone(mono::EventHandler& eventHandler);
        ~TestZone();
                
        virtual void OnLoad(mono::ICameraPtr camera);
        virtual void OnUnload();
        
        void SpawnEntity(const game::SpawnEntityEvent& event);
        void SpawnPhysicsEntity(const game::SpawnPhysicsEntityEvent& event);
        void OnRemoveEntityById(const game::RemoveEntityByIdEvent& event);
        void OnShockwaveEvent(const game::ShockwaveEvent& event);
        void OnDamageEvent(const game::DamageEvent& event);

    private:

        virtual void AddPhysicsEntity(const mono::IPhysicsEntityPtr& entity, int layer);
        virtual void RemovePhysicsEntity(const mono::IPhysicsEntityPtr& entity);
        virtual void AddEntity(const mono::IEntityPtr& entity, int layer);
        virtual void RemoveEntity(const mono::IEntityPtr& entity);


        mono::EventToken<game::SpawnEntityEvent> mSpawnEntityToken;
        mono::EventToken<game::SpawnPhysicsEntityEvent> mSpawnPhysicsEntityToken;
        mono::EventToken<game::RemoveEntityByIdEvent> mRemoveEntityByIdToken;
        mono::EventToken<game::ShockwaveEvent> mShockwaveEventToken;
        mono::EventToken<game::DamageEvent> mDamageEventToken;
        
        mono::EventHandler& mEventHandler;
        Spawner m_spawner;
        DamageController m_damageController;
        
        mono::ISoundPtr m_backgroundMusic;
        std::shared_ptr<Network::ISocket> m_socket;
        std::shared_ptr<Network::ISocket> m_outSocket;
    };
}

