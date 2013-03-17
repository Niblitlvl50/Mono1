//
//  TestZone.cpp
//  Mono1
//
//  Created by Niblit on 2012-04-06.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "TestZone.h"
#include "TriangleObject.h"
#include "OscillatingLine.h"
#include "AnimatedDude.h"
#include "Explosion.h"
#include "Shuttle.h"
#include "Moon.h"
#include "ICamera.h"
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

#include <algorithm>
#include <cmath>


using namespace game;

namespace
{
    struct ZoneBounds : mono::EntityBase
    {
        ZoneBounds(const Math::Quad& bounds)
            : mBounds(bounds)
        {
            mPhysics.body = cm::Factory::CreateBody(INFINITY, INFINITY);
            
            const float radius = 2.0f;
            
            const Math::Vector2f first = mBounds.mA;
            const Math::Vector2f second = Math::Vector2f(mBounds.mA.mX, mBounds.mB.mY);
            const Math::Vector2f third = mBounds.mB;
            const Math::Vector2f fourth = Math::Vector2f(mBounds.mB.mX, mBounds.mA.mY);
            
            mPhysics.shapes.push_back(cm::Factory::CreateShape(mPhysics.body, first, second, radius, true));
            mPhysics.shapes.push_back(cm::Factory::CreateShape(mPhysics.body, second, third, radius, true));
            mPhysics.shapes.push_back(cm::Factory::CreateShape(mPhysics.body, third, fourth, radius, true));
            mPhysics.shapes.push_back(cm::Factory::CreateShape(mPhysics.body, fourth, first, radius, true));
            
            using namespace std::tr1::placeholders;
            std::for_each(mPhysics.shapes.begin(), mPhysics.shapes.end(), std::tr1::bind(&cm::IShape::SetElasticity, _1, 0.9f));
        }
        virtual void Draw(mono::IRenderer&) const
        {
            mono::DrawQuad(mBounds);
        }
        virtual Math::Quad BoundingBox() const
        {
            return mBounds;
        }
        virtual void Update(unsigned int delta)
        { }
        
        const Math::Quad mBounds;
        cm::Object mPhysics;
    };
    
    struct GravityUpdater : mono::IUpdatable
    {
        GravityUpdater(mono::PhysicsZone& zone, std::tr1::shared_ptr<Moon> moon1, std::tr1::shared_ptr<Moon> moon2)
            : mZone(zone),
              mMoon1(moon1),
              mMoon2(moon2),
              mElapsedTime(0)
        { }
        virtual void doUpdate(unsigned int delta)
        {
            mElapsedTime += delta;
            if(mElapsedTime < 10)
                return;
            
            using namespace std::tr1::placeholders;
            mZone.ForEachBody(std::tr1::bind(&GravityUpdater::GravityBodyFunc, this, _1));
            mElapsedTime = 0;
        }
        void GravityBodyFunc(cm::IBodyPtr body)
        {
            Math::Vector2f impulse;
            
            Math::Vector2f newPos = body->GetPosition() - mMoon1->Position();
            const float distance = Math::Length(newPos);
            if(distance < 300.0f)
            {
                const float gravity = 1e4f;
                const float value = -gravity / (distance * sqrtf(distance));
            
                Math::Normalize(newPos);
                newPos *= value;
                
                impulse += newPos;
            }
            
            Math::Vector2f newPos2 = body->GetPosition() - mMoon2->Position();
            const float distance2 = Math::Length(newPos2);
            if(distance2 < 200.0f)
            {
                const float gravity = 1e4f;
                const float value = -gravity / (distance2 * sqrtf(distance2));
                
                Math::Normalize(newPos2);
                newPos2 *= value;
                
                impulse += newPos2;
            }
            
            body->ApplyImpulse(impulse, Math::Vector2f(0.0, 0.0));
        }
        
        mono::PhysicsZone& mZone;
        std::tr1::shared_ptr<Moon> mMoon1;
        std::tr1::shared_ptr<Moon> mMoon2;
        unsigned int mElapsedTime;
    };
    
}

TestZone::TestZone()
    : PhysicsZone(Math::Vector2f(0.0f, 0.0f))
{
    using namespace std::tr1::placeholders;
    
    const game::SpawnEntityEventFunc spawnEntityFunc = std::tr1::bind(&TestZone::SpawnEntity, this, _1);
    mSpawnEntityToken = mono::EventHandler::AddListener(spawnEntityFunc);
    
    const game::SpawnPhysicsEntityFunc spawnPhysicsFunc = std::tr1::bind(&TestZone::SpawnPhysicsEntity, this, _1);
    mSpawnPhysicsEntityToken = mono::EventHandler::AddListener(spawnPhysicsFunc);
}

TestZone::~TestZone()
{
    mono::EventHandler::RemoveListener(mSpawnEntityToken);
    mono::EventHandler::RemoveListener(mSpawnPhysicsEntityToken);
}

void TestZone::OnLoad(mono::ICameraPtr camera)
{    
    mono::IEntityPtr dude(new AnimatedDude(100.0f, 50.0f));
    AddEntityToLayer(MIDDLEGROUND, dude);
    
    std::tr1::shared_ptr<ZoneBounds> bounds(new ZoneBounds(Math::Quad(0.0f, 0.0f, 1000.0f, 600.0f)));
    AddPhysicsObject(bounds->mPhysics, false);
    AddEntityToLayer(BACKGROUND, bounds);
        
    std::tr1::shared_ptr<Shuttle> shuttle(new Shuttle(100.0f, 100.0f));
    AddPhysicsObject(shuttle->mPhysicsObject, true);
    AddEntityToLayer(FOREGROUND, shuttle);

    std::tr1::shared_ptr<Moon> moon1(new Moon(550.0f, 300.0f, 100.0f));
    AddPhysicsObject(moon1->mPhysicsObject, false);
    AddEntityToLayer(BACKGROUND, moon1);
    
    std::tr1::shared_ptr<Moon> moon2(new Moon(200.0f, 400.0f, 50.0f));
    AddPhysicsObject(moon2->mPhysicsObject, false);
    AddEntityToLayer(BACKGROUND, moon2);
    
    AddEntityToLayer(BACKGROUND, mono::IEntityPtr(new TriangleObject));
    AddEntityToLayer(FOREGROUND, mono::IEntityPtr(new OscillatingLine));
    AddEntityToLayer(FOREGROUND, mono::IEntityPtr(new Explosion));
    
    AddUpdatable(mono::IUpdatablePtr(new GravityUpdater(*this, moon1, moon2)));
        
    camera->SetPosition(shuttle->Position());
    camera->Follow(shuttle);
}

void TestZone::OnUnload()
{ }

void TestZone::SpawnEntity(const game::SpawnEntityEvent& event)
{
    AddEntityToLayer(FOREGROUND, event.mEntity);
}

void TestZone::SpawnPhysicsEntity(const game::SpawnPhysicsEntityEvent& event)
{
    AddEntityToLayer(FOREGROUND, event.mEntity);
    AddPhysicsObject(event.mPhysics, true);
}
