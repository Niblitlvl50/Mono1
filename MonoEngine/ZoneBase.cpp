//
//  ZoneBase.cpp
//  Mono1
//
//  Created by Niblit on 2012-04-21.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "ZoneBase.h"
#include "IRenderer.h"
#include "IEntity.h"
#include "Utils.h"

#include <stdexcept>

using namespace mono;

ZoneBase::ZoneBase()
{
    // Make sure the three layers exists.
    mDrawables[BACKGROUND]   = std::vector<IDrawablePtr>();
    mDrawables[MIDDLEGROUND] = std::vector<IDrawablePtr>();
    mDrawables[FOREGROUND]   = std::vector<IDrawablePtr>();
}

void ZoneBase::Accept(IRenderer& renderer)
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

void ZoneBase::DoPreAccept()
{
    const auto func = [](const IEntityPtr& entity) {
        return entity->RemoveMe();
    };

    auto part_it = std::partition(mEntities.begin(), mEntities.end(), func);
    if(part_it == mEntities.end())
        return;

    for(auto it = part_it, end = mEntities.end(); it != end; ++it)
    {
        RemoveDrawable(*it);
        RemoveUpdatable(*it);
    }

    mEntities.erase(part_it, mEntities.end());
}

void ZoneBase::AddEntity(const IEntityPtr& entity, int layer)
{
    AddDrawable(entity, layer);
    AddUpdatable(entity);

    mEntities.push_back(entity);
}

void ZoneBase::RemoveEntity(const IEntityPtr& entity)
{
    RemoveDrawable(entity);
    RemoveUpdatable(entity);

    const bool result = mono::FindAndRemove(mEntities, entity);
    if(!result)
        throw std::runtime_error("ZoneBase - Unable to remove entity");
}

void ZoneBase::AddUpdatable(const IUpdatablePtr& updatable)
{
    mUpdatables.push_back(updatable);
}

void ZoneBase::RemoveUpdatable(const IUpdatablePtr& updatable)
{
    const bool result = mono::FindAndRemove(mUpdatables, updatable);
    if(!result)
        throw std::runtime_error("ZoneBase - Unable to remove updatable");
}

void ZoneBase::AddDrawable(const IDrawablePtr& drawable, int layer)
{
    auto layerIt = mDrawables.find(layer);
    if(layerIt == mDrawables.end())
        throw std::runtime_error("ZoneBase - Adding drawable to missing layer");
    
    layerIt->second.push_back(drawable);
}

void ZoneBase::RemoveDrawable(const IDrawablePtr& drawable)
{
    bool result = false;
    for(auto& layer : mDrawables)
    {
        result = mono::FindAndRemove(layer.second, drawable);
        if(result)
            break;
    }
    
    if(!result)
        throw std::runtime_error("ZoneBase - Unable to remove drawable");
}
