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
#include "ICamera.h"
#include "EntityBase.h"
#include "Quad.h"
#include "CMObject.h"
#include "CMFactory.h"
#include "CMIShape.h"

#include <algorithm>


using namespace game;

namespace
{
    struct SetElasticity
    {
        SetElasticity(float value)
            : mValue(value)
        { }
        void operator()(cm::IShapePtr shape) const
        {
            shape->SetElasticity(mValue);
        }
        const float mValue;
    };
    
    
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
            
            std::for_each(mPhysics.shapes.begin(), mPhysics.shapes.end(), SetElasticity(0.9f));
        }
        virtual void Draw(mono::IRenderer&) const
        { }
        virtual Math::Quad BoundingBox() const
        {
            return mBounds;
        }
        virtual void Update(unsigned int delta)
        { }
        
        const Math::Quad mBounds;
        cm::Object mPhysics;
    };
    
}

TestZone::TestZone()
    : PhysicsZone(Math::Vector2f(-1.0f, -5.0f))
{ }

void TestZone::OnLoad(mono::ICameraPtr camera)
{    
    mono::IEntityPtr dude(new AnimatedDude(100.0f, 50.0f));
    AddEntityToLayer(MIDDLEGROUND, dude);
    
    std::tr1::shared_ptr<ZoneBounds> bounds(new ZoneBounds(Math::Quad(0.0f, 0.0f, 800.0f, 500.0f)));
    AddPhysicsObject(bounds->mPhysics, false);
    AddEntityToLayer(BACKGROUND, bounds);
        
    std::tr1::shared_ptr<Shuttle> shuttle(new Shuttle(100.0f, 100.0f));
    AddPhysicsObject(shuttle->mPhysicsObject, true);
    AddEntityToLayer(FOREGROUND, shuttle);
        
    AddEntityToLayer(BACKGROUND, mono::IEntityPtr(new TriangleObject));
    AddEntityToLayer(FOREGROUND, mono::IEntityPtr(new OscillatingLine));
    AddEntityToLayer(FOREGROUND, mono::IEntityPtr(new Explosion));
    
    camera->SetPosition(dude->Position());
    camera->Follow(dude);
}

void TestZone::OnUnload()
{ }

