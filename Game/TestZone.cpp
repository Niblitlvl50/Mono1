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
#include "MathFwd.h"

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
            mono::DrawQuad(mBounds);
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
        GravityUpdater(mono::PhysicsZone* zone, mono::IEntityPtr moon1, mono::IEntityPtr moon2)
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
        
        mono::PhysicsZone* mZone;
        mono::IEntityPtr mMoon1;
        mono::IEntityPtr mMoon2;
        unsigned int mElapsedTime;
    };
    
}

TestZone::TestZone()
    : PhysicsZone(math::Vector2f(0.0f, 0.0f))
{    
    const game::SpawnEntityFunc spawnEntityFunc = std::bind(&TestZone::SpawnEntity, this, _1);
    mSpawnEntityToken = mono::EventHandler::AddListener(spawnEntityFunc);
    
    const game::SpawnPhysicsEntityFunc spawnPhysicsFunc = std::bind(&TestZone::SpawnPhysicsEntity, this, _1);
    mSpawnPhysicsEntityToken = mono::EventHandler::AddListener(spawnPhysicsFunc);
    
    const game::RemoveEntityFunc removeEntityFunc = std::bind(&TestZone::RemoveEntity, this, _1);
    mRemoveEntityToken = mono::EventHandler::AddListener(removeEntityFunc);
    
    const game::RemovePhysicsEntityFunc removePhysicsFunc = std::bind(&TestZone::RemovePhysicsEntity, this, _1);
    mRemovePhysicsEntityToken = mono::EventHandler::AddListener(removePhysicsFunc);
}

TestZone::~TestZone()
{
    mono::EventHandler::RemoveListener(mSpawnEntityToken);
    mono::EventHandler::RemoveListener(mSpawnPhysicsEntityToken);
    mono::EventHandler::RemoveListener(mRemoveEntityToken);
    mono::EventHandler::RemoveListener(mRemovePhysicsEntityToken);
}

void TestZone::OnLoad(mono::ICameraPtr camera)
{    
    AddEntityToLayer(mono::IEntityPtr(new AnimatedDude(100.0f, 50.0f)), MIDDLEGROUND);
    
    std::shared_ptr<ZoneBounds> bounds(new ZoneBounds(math::Quad(0.0f, 0.0f, 1000.0f, 600.0f)));
    AddPhysicsEntityToLayer(bounds, BACKGROUND);
        
    std::shared_ptr<Shuttle> shuttle(new Shuttle(100.0f, 100.0f));
    AddPhysicsEntityToLayer(shuttle, FOREGROUND);

    std::shared_ptr<Moon> moon1(new Moon(550.0f, 300.0f, 100.0f));
    AddPhysicsEntityToLayer(moon1, FOREGROUND);
    
    std::shared_ptr<Moon> moon2(new Moon(200.0f, 400.0f, 50.0f));
    AddPhysicsEntityToLayer(moon2, FOREGROUND);
    
    AddEntityToLayer(mono::IEntityPtr(new TriangleObject), BACKGROUND);
    AddEntityToLayer(mono::IEntityPtr(new OscillatingLine), FOREGROUND);
    
    AddUpdatable(mono::IUpdatablePtr(new GravityUpdater(this, moon1, moon2)));
        
    camera->SetPosition(shuttle->Position());
    camera->Follow(shuttle);
}

void TestZone::OnUnload()
{ }

void TestZone::SpawnEntity(const game::SpawnEntityEvent& event)
{
    AddEntityToLayer(event.mEntity, FOREGROUND);
}

void TestZone::SpawnPhysicsEntity(const game::SpawnPhysicsEntityEvent& event)
{
    AddPhysicsEntityToLayer(event.mEntity, FOREGROUND);
}

void TestZone::RemoveEntity(const game::RemoveEntityEvent& event)
{
    RemoveEntity(event.mEntity);
}

void TestZone::RemovePhysicsEntity(const game::RemovePhysicsEntityEvent& event)
{
    RemovePhysicsEntity(event.mEntity);
}



