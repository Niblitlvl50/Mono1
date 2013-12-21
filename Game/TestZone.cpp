//
//  TestZone.cpp
//  Mono1
//
//  Created by Niblit on 2012-04-06.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "TestZone.h"
#include "ICamera.h"

#include "TriangleObject.h"
#include "OscillatingLine.h"
#include "AnimatedDude.h"
#include "Shuttle.h"
#include "Moon.h"
#include "PathPoint.h"
#include "EntityBase.h"
#include "Quad.h"
#include "CMObject.h"
#include "CMFactory.h"
#include "CMIShape.h"
#include "CMIBody.h"
#include "RenderUtils.h"

#include "EventHandler.h"
#include "SpawnEntityEvent.h"
#include "SpawnPhysicsEntityEvent.h"
#include "RemoveEntityEvent.h"
#include "RemovePhysicsEntityEvent.h"
#include "Math.h"

#include <algorithm>
#include <cmath>


using namespace game;
using namespace std::placeholders;


namespace
{
    struct ZoneBounds : mono::PhysicsEntityBase
    {
        ZoneBounds(const math::Quad& bounds)
            : mBounds(bounds)
        {
            mPhysicsObject.body = cm::Factory::CreateStaticBody();
            
            const float radius = 2.0f;
            
            const math::Vector2f first = mBounds.mA;
            const math::Vector2f second = math::Vector2f(mBounds.mA.mX, mBounds.mB.mY);
            const math::Vector2f third = mBounds.mB;
            const math::Vector2f fourth = math::Vector2f(mBounds.mB.mX, mBounds.mA.mY);
            
            mPhysicsObject.shapes.push_back(cm::Factory::CreateShape(mPhysicsObject.body, first, second, radius));
            mPhysicsObject.shapes.push_back(cm::Factory::CreateShape(mPhysicsObject.body, second, third, radius));
            mPhysicsObject.shapes.push_back(cm::Factory::CreateShape(mPhysicsObject.body, third, fourth, radius));
            mPhysicsObject.shapes.push_back(cm::Factory::CreateShape(mPhysicsObject.body, fourth, first, radius));
            
            std::for_each(mPhysicsObject.shapes.begin(), mPhysicsObject.shapes.end(), std::bind(&cm::IShape::SetElasticity, _1, 0.9f));
        }
        virtual void Draw(mono::IRenderer&) const
        {
            mono::DrawQuad(mBounds, mono::Color(1, 1, 1, 1), false);
        }
        virtual math::Quad BoundingBox() const
        {
            return mBounds;
        }
        virtual void Update(unsigned int delta)
        { }
        
        const math::Quad mBounds;
    };
    
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
                const float value = -gravity / (distance * sqrtf(distance));
            
                math::Normalize(newPos);
                newPos *= value;
                
                impulse += newPos;
            }
            
            math::Vector2f newPos2 = body->GetPosition() - mMoon2->Position();
            const float distance2 = math::Length(newPos2);
            if(distance2 < 200.0f)
            {
                const float gravity = 1e4f;
                const float value = -gravity / (distance2 * sqrtf(distance2));
                
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
    
    std::shared_ptr<ZoneBounds> bounds = std::make_shared<ZoneBounds>(math::Quad(0.0f, 0.0f, 1000.0f, 600.0f));
    AddPhysicsEntity(bounds, mono::BACKGROUND);
        
    std::shared_ptr<Shuttle> shuttle = std::make_shared<Shuttle>(100.0f, 100.0f, eventHandler);
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



