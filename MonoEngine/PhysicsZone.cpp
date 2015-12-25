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
#include "IPhysicsEntity.h"

#include "Utils.h"

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
{
    // Add space physics as an updatable
    AddUpdatable(mPhysics);
}

void PhysicsZone::DoPreAccept()
{
    ZoneBase::DoPreAccept();

    const auto func = [](const mono::IEntityPtr& entity) {
        return !entity->RemoveMe();
    };

    auto part_it = std::partition(mPhysicsEntities.begin(), mPhysicsEntities.end(), func);
    if(part_it == mPhysicsEntities.end())
        return;
    
    // Handle clean up for physics entities, do this in two stages just to remove
    // the drawables first and then erase the entries from the mEntities vector.
    for(auto it = part_it, end = mPhysicsEntities.end(); it != end; ++it)
    {
        IPhysicsEntityPtr entity = *it;

        cm::Object& object = entity->GetPhysics();
        mPhysics->mSpace.RemoveBody(object.body);

        for(auto& shape : object.shapes)
            mPhysics->mSpace.RemoveShape(shape);

        RemoveDrawable(entity);
        RemoveUpdatable(entity);
    }

    mPhysicsEntities.erase(part_it, mPhysicsEntities.end());
}

void PhysicsZone::ForEachBody(const cm::BodyFunc& func)
{
    mPhysics->mSpace.ForEachBody(func);
}

void PhysicsZone::AddPhysicsEntity(const mono::IPhysicsEntityPtr& entity, int layer)
{
    AddDrawable(entity, layer);
    AddUpdatable(entity);
    
    const cm::Object& object = entity->GetPhysics();
    mPhysics->mSpace.AddBody(object.body);
    
    for(auto& shape : object.shapes)
        mPhysics->mSpace.AddShape(shape);
    
    mPhysicsEntities.push_back(entity);
}

void PhysicsZone::RemovePhysicsEntity(const mono::IPhysicsEntityPtr& entity)
{
    RemoveDrawable(entity);
    RemoveUpdatable(entity);

    const bool result = FindAndRemove(mPhysicsEntities, entity);
    if(result)
    {
        cm::Object& object = entity->GetPhysics();
        mPhysics->mSpace.RemoveBody(object.body);
        
        for(auto& shape : object.shapes)
            mPhysics->mSpace.RemoveShape(shape);
    }
    else
    {
        throw std::runtime_error("PhysicsZone - Unable to remove physics entity");
    }
}



