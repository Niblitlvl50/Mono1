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
#include "CMIShape.h"
#include "Vector2f.h"
#include "IEntity.h"

using namespace mono;

struct PhysicsZone::PhysicsImpl : IUpdatable
{
    PhysicsImpl(const math::Vector2f& gravity)
        : mSpace(gravity)
    { }
    void doUpdate(unsigned int delta)
    {
        const float floatDelta = delta;
        mSpace.Tick(floatDelta / 1000.0f);
    }
    cm::Space mSpace;
};

PhysicsZone::PhysicsZone(const math::Vector2f& gravity)
    : mPhysics(new PhysicsImpl(gravity))
{ }

void PhysicsZone::Accept(IRenderer& renderer)
{
    renderer.AddUpdatable(mPhysics);
    ZoneBase::Accept(renderer);
}

void PhysicsZone::ForEachBody(const cm::BodyFunc& func)
{
    mPhysics->mSpace.ForEachBody(func);
}

void PhysicsZone::AddPhysicsEntityToLayer(mono::IPhysicsEntityPtr entity, LayerId layer)
{
    cm::Object& object = entity->GetPhysics();
    mPhysics->mSpace.AddBody(object.body);
    
    for(cm::IShapeCollection::iterator it = object.shapes.begin(), end = object.shapes.end(); it != end; ++it)
        mPhysics->mSpace.AddShape(*it);
    
    AddEntityToLayer(entity, layer);
}

void PhysicsZone::RemovePhysicsEntity(mono::IPhysicsEntityPtr entity)
{
    cm::Object& object = entity->GetPhysics();
    mPhysics->mSpace.RemoveBody(object.body);
    
    for(cm::IShapeCollection::iterator it = object.shapes.begin(), end = object.shapes.end(); it != end; ++it)
        mPhysics->mSpace.RemoveShape(*it);
    
    RemoveEntity(entity);
}



