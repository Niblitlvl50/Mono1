//
//  PhysicsZone.cpp
//  Mono1
//
//  Created by Niblit on 2012-08-23.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "PhysicsZone.h"
#include "IRenderer.h"
#include "IUpdatable.h"
#include "CMSpace.h"
#include "CMObject.h"
#include "Vector2f.h"

using namespace mono;

struct PhysicsZone::PhysicsImpl : IUpdatable
{
    PhysicsImpl(const Math::Vector2f& gravity)
        : mSpace(gravity)
    { }
    void doUpdate(unsigned int delta)
    {
        const float floatDelta = delta;
        mSpace.Tick(floatDelta / 1000.0f);
    }

    cm::Space mSpace;
};

PhysicsZone::PhysicsZone(const Math::Vector2f& gravity)
    : mPhysics(new PhysicsImpl(gravity))
{ }

void PhysicsZone::Accept(IRenderer& renderer)
{
    renderer.AddUpdatable(mPhysics);
    ZoneBase::Accept(renderer);
}

void PhysicsZone::AddBody(cm::IBodyPtr body)
{
    mPhysics->mSpace.AddBody(body);
}
void PhysicsZone::AddShape(cm::IShapePtr shape)
{
    mPhysics->mSpace.AddShape(shape);
}

void PhysicsZone::AddPhysicsObject(cm::Object& object, bool addBody)
{
    if(addBody)
        AddBody(object.body);
    
    for(cm::IShapeCollection::iterator it = object.shapes.begin(), end = object.shapes.end(); it != end; ++it)
        AddShape(*it);
}
