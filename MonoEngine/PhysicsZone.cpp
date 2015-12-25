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
    mUpdatables.push_back(mPhysics);

    // Make sure the three layers exists.
    mDrawables[BACKGROUND]   = std::vector<IDrawablePtr>();
    mDrawables[MIDDLEGROUND] = std::vector<IDrawablePtr>();
    mDrawables[FOREGROUND]   = std::vector<IDrawablePtr>();
}

void PhysicsZone::Accept(IRenderer& renderer)
{
    DoPreAccept();

    const auto addDrawableFunc = [&renderer](const IDrawablePtr& drawable) {
        renderer.AddDrawable(drawable);
    };
    
    const auto addUpdatableFunc = [&renderer](const IUpdatablePtr& updatable) {
        renderer.AddUpdatable(updatable);
    };
    
    std::for_each(mDrawables[BACKGROUND].begin(),   mDrawables[BACKGROUND].end(),   addDrawableFunc);
    std::for_each(mDrawables[MIDDLEGROUND].begin(), mDrawables[MIDDLEGROUND].end(), addDrawableFunc);
    std::for_each(mDrawables[FOREGROUND].begin(),   mDrawables[FOREGROUND].end(),   addDrawableFunc);
    
    std::for_each(mUpdatables.begin(), mUpdatables.end(), addUpdatableFunc);
}

void PhysicsZone::DoPreAccept()
{    
    const auto removeFunc = [](const mono::IEntityPtr& entity) {
        return entity->RemoveMe();
    };
    
    // Handle clean up for physics entities, do this in two stages just to remove
    // the drawables first and then erase the entries from the mEntities vector.
    for(auto& entity : mPhysicsEntities)
    {
        if(!entity->RemoveMe())
            continue;
        
        cm::Object& object = entity->GetPhysics();
        mPhysics->mSpace.RemoveBody(object.body);

        for(auto& shape : object.shapes)
            mPhysics->mSpace.RemoveShape(shape);

        RemoveDrawable(entity);
        RemoveUpdatable(entity);
    }
    
    const auto physIt = std::remove_if(mPhysicsEntities.begin(), mPhysicsEntities.end(), removeFunc);
    if(physIt != mPhysicsEntities.end())
        mPhysicsEntities.erase(physIt, mPhysicsEntities.end());
    
    
    // Handle clean up for entities, do this in two stages just to remove
    // the drawables first and then erase the entries from the mEntities vector.
    for(auto& entity : mEntities)
    {
        if(!entity->RemoveMe())
            continue;

        RemoveDrawable(entity);
        RemoveUpdatable(entity);
    }
    
    const auto removeIt = std::remove_if(mEntities.begin(), mEntities.end(), removeFunc);
    if(removeIt != mEntities.end())
        mEntities.erase(removeIt, mEntities.end());
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

void PhysicsZone::AddEntity(const mono::IEntityPtr& entity, int layer)
{
    AddDrawable(entity, layer);
    AddUpdatable(entity);

    mEntities.push_back(entity);
}

void PhysicsZone::RemoveEntity(const mono::IEntityPtr& entity)
{
    RemoveDrawable(entity);
    RemoveUpdatable(entity);
    
    const bool result = FindAndRemove(mEntities, entity);
    if(!result)
        throw std::runtime_error("PhysicsZone - Unable to remove entity");
}

void PhysicsZone::AddDrawable(const mono::IDrawablePtr& drawable, int layer)
{
    auto layerIt = mDrawables.find(layer);
    if(layerIt == mDrawables.end())
        throw std::runtime_error("PhysicsZone - Adding drawable to missing layer");
    
    layerIt->second.push_back(drawable);
}

void PhysicsZone::RemoveDrawable(const mono::IDrawablePtr& drawable)
{
    bool result = false;
    for(auto& layer : mDrawables)
    {
        result = FindAndRemove(layer.second, drawable);
        if(result)
            break;
    }
    
    if(!result)
        throw std::runtime_error("PhysicsZone - Unable to remove drawable");
}

void PhysicsZone::AddUpdatable(const mono::IUpdatablePtr& updatable)
{
    mUpdatables.push_back(updatable);
}

void PhysicsZone::RemoveUpdatable(const mono::IUpdatablePtr& updatable)
{
    const bool result = mono::FindAndRemove(mUpdatables, updatable);
    if(!result)
        throw std::runtime_error("PhysicsZone - Unable to remove updatable");
}




