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

#include "Utils.h"


using namespace mono;

namespace
{
    bool RemoveIfDead(mono::IPhysicsEntityPtr entity)
    {
        return entity->RemoveMe();
    }
}

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

void PhysicsZone::DoPreAccept()
{
    for(auto it = mPhysicsEntities.begin(), end = mPhysicsEntities.end(); it != end; ++it)
    {
        mono::IPhysicsEntityPtr entity = *it;
        if(entity->RemoveMe())
        {
            cm::Object& object = entity->GetPhysics();
        
            for(auto shapeIt = object.shapes.begin(), shapeEnd = object.shapes.end(); shapeIt != shapeEnd; ++shapeIt)
                mPhysics->mSpace.RemoveShape(*shapeIt);            
        
            mPhysics->mSpace.RemoveBody(object.body);        
        }
    }
        
    auto it = std::remove_if(mPhysicsEntities.begin(), mPhysicsEntities.end(), RemoveIfDead);
    if(it != mPhysicsEntities.end())
        mPhysicsEntities.erase(it, mPhysicsEntities.end());
    
    ZoneBase::DoPreAccept();
}

void PhysicsZone::ForEachBody(const cm::BodyFunc& func)
{
    mPhysics->mSpace.ForEachBody(func);
}

void PhysicsZone::AddPhysicsEntityToLayer(mono::IPhysicsEntityPtr entity, LayerId layer)
{
    mPhysicsEntities.push_back(entity);
    
    cm::Object& object = entity->GetPhysics();
    mPhysics->mSpace.AddBody(object.body);
    
    for(auto it = object.shapes.begin(), end = object.shapes.end(); it != end; ++it)
        mPhysics->mSpace.AddShape(*it);
    
    AddEntityToLayer(entity, layer);
}

void PhysicsZone::RemovePhysicsEntity(mono::IPhysicsEntityPtr entity)
{
    const bool result = FindAndRemove(mPhysicsEntities, entity);
    
    cm::Object& object = entity->GetPhysics();
    
    for(auto it = object.shapes.begin(), end = object.shapes.end(); it != end; ++it)
        mPhysics->mSpace.RemoveShape(*it);
    
    mPhysics->mSpace.RemoveBody(object.body);

    RemoveEntity(entity);
}



