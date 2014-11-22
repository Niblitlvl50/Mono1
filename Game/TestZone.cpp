//
//  TestZone.cpp
//  Mono1
//
//  Created by Niblit on 2012-04-06.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "TestZone.h"
#include "ICamera.h"

#include "PhysicsGrid.h"
#include "TriangleObject.h"
#include "OscillatingLine.h"
#include "AnimatedDude.h"
#include "Shuttle.h"
#include "Moon.h"
#include "PathPoint.h"

#include "Quad.h"

#include "EventHandler.h"
#include "SpawnEntityEvent.h"
#include "SpawnPhysicsEntityEvent.h"
#include "RemoveEntityEvent.h"
#include "RemovePhysicsEntityEvent.h"
#include "ShockwaveEvent.h"

#include <cmath>


using namespace game;
using namespace std::placeholders;


namespace
{
    struct GravityUpdater : mono::IUpdatable
    {
        GravityUpdater(mono::IPhysicsZone* zone, mono::IEntityPtr moon1, mono::IEntityPtr moon2)
            : mZone(zone),
              mMoon1(moon1),
              mMoon2(moon2),
              mElapsedTime(0)
        { }
        virtual void doUpdate(unsigned int delta)
        {
            mElapsedTime += delta;
            if(mElapsedTime < 16)
                return;
            
            mZone->ForEachBody(std::bind(&GravityUpdater::GravityBodyFunc, this, _1));
            mElapsedTime = 0;
        }
        void GravityBodyFunc(cm::IBodyPtr body)
        {
            math::Vector2f impulse;
            
            math::Vector2f newPos = body->GetPosition() - mMoon1->Position();
            const float distance = math::Length(newPos);
            if(distance < 300.0f)
            {
                const float gravity = 1e4f;
                const float value = -gravity / (distance * std::sqrtf(distance));
            
                math::Normalize(newPos);
                newPos *= value;
                
                impulse += newPos;
            }
            
            math::Vector2f newPos2 = body->GetPosition() - mMoon2->Position();
            const float distance2 = math::Length(newPos2);
            if(distance2 < 200.0f)
            {
                const float gravity = 1e4f;
                const float value = -gravity / (distance2 * std::sqrtf(distance2));
                
                math::Normalize(newPos2);
                newPos2 *= value;
                
                impulse += newPos2;
            }
            
            body->ApplyImpulse(impulse, math::zeroVec);
        }
        
        mono::IPhysicsZone* mZone;
        mono::IEntityPtr mMoon1;
        mono::IEntityPtr mMoon2;
        unsigned int mElapsedTime;
    };

    void ApplyShockwave(cm::IBodyPtr body, const math::Vector2f& position, float magnitude)
    {
        math::Vector2f unit = body->GetPosition() - position;
        math::Normalize(unit);
        
        const math::Vector2f& impulse = unit * magnitude;
        body->ApplyImpulse(impulse, math::zeroVec);
    }
    
}

TestZone::TestZone()
    : PhysicsZone(math::Vector2f(0.0f, 0.0f))
{ }

void TestZone::AddEventListeners()
{
    const game::SpawnEntityFunc spawnEntityFunc = std::bind(&TestZone::SpawnEntity, this, _1);
    mSpawnEntityToken = mEventHandler->AddListener(spawnEntityFunc);
    
    const game::SpawnPhysicsEntityFunc spawnPhysicsFunc = std::bind(&TestZone::SpawnPhysicsEntity, this, _1);
    mSpawnPhysicsEntityToken = mEventHandler->AddListener(spawnPhysicsFunc);
    
    const game::RemoveEntityFunc removeEntityFunc = std::bind(&TestZone::RemoveEntity, this, _1);
    mRemoveEntityToken = mEventHandler->AddListener(removeEntityFunc);
    
    const game::RemovePhysicsEntityFunc removePhysicsFunc = std::bind(&TestZone::RemovePhysicsEntity, this, _1);
    mRemovePhysicsEntityToken = mEventHandler->AddListener(removePhysicsFunc);

    const game::ShockwaveEventFunc shockwaveFunc = std::bind(&TestZone::OnShockwaveEvent, this, _1);
    mShockwaveEventToken = mEventHandler->AddListener(shockwaveFunc);
}

void TestZone::RemoveEventListeners()
{
    mEventHandler->RemoveListener(mSpawnEntityToken);
    mEventHandler->RemoveListener(mSpawnPhysicsEntityToken);
    mEventHandler->RemoveListener(mRemoveEntityToken);
    mEventHandler->RemoveListener(mRemovePhysicsEntityToken);
}

void TestZone::OnLoad(mono::ICameraPtr camera, mono::EventHandler& eventHandler)
{
    mEventHandler = &eventHandler;
    AddEventListeners();
    
    AddEntity(std::make_shared<AnimatedDude>(100.0f, 50.0f, eventHandler), mono::MIDDLEGROUND);
    
    std::shared_ptr<PhysicsGrid> bounds = std::make_shared<PhysicsGrid>(math::Quad(-1000.0f, -1000.0f, 1000.0f, 1000.0f));
    AddPhysicsEntity(bounds, mono::BACKGROUND);
        
    std::shared_ptr<Shuttle> shuttle = std::make_shared<Shuttle>(-100.0f, 950.0f, eventHandler);
    AddPhysicsEntity(shuttle, mono::FOREGROUND);

    std::shared_ptr<Moon> moon1 = std::make_shared<Moon>(550.0f, 300.0f, 100.0f);
    AddPhysicsEntity(moon1, mono::FOREGROUND);
    
    std::shared_ptr<Moon> moon2 = std::make_shared<Moon>(200.0f, 400.0f, 50.0f);
    AddPhysicsEntity(moon2, mono::FOREGROUND);
    
    AddEntity(std::make_shared<TriangleObject>(), mono::BACKGROUND);
    AddEntity(std::make_shared<OscillatingLine>(), mono::FOREGROUND);
    AddEntity(std::make_shared<PathPoint>(), mono::BACKGROUND);
    
    AddUpdatable(std::make_shared<GravityUpdater>(this, moon1, moon2));
        
    camera->SetPosition(shuttle->Position());
    camera->Follow(shuttle);
}

void TestZone::OnUnload()
{
    RemoveEventListeners();
}

void TestZone::SpawnEntity(const game::SpawnEntityEvent& event)
{
    AddEntity(event.mEntity, mono::FOREGROUND);
}

void TestZone::SpawnPhysicsEntity(const game::SpawnPhysicsEntityEvent& event)
{
    AddPhysicsEntity(event.mEntity, mono::FOREGROUND);
}

void TestZone::RemoveEntity(const game::RemoveEntityEvent& event)
{
    RemoveEntity(event.mEntity);
}

void TestZone::RemovePhysicsEntity(const game::RemovePhysicsEntityEvent& event)
{
    RemovePhysicsEntity(event.mEntity);
}

void TestZone::OnShockwaveEvent(const game::ShockwaveEvent& event)
{
    const auto func = std::bind(ApplyShockwave, _1, event.mPosition, event.mMagnitude);
    this->ForEachBody(func);
}



