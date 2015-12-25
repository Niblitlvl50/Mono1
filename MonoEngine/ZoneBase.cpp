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

void ZoneBase::Accept(IRenderer& renderer)
{
    DoPreAccept();

    for(auto& layer : mDrawables)
    {
        for(auto& drawable : layer.second)
            renderer.AddDrawable(drawable);
    }

    for(auto& updatable : mUpdatables)
        renderer.AddUpdatable(updatable);
}

void ZoneBase::DoPreAccept()
{
    const auto func = [](const IEntityPtr& entity) {
        return !entity->RemoveMe();
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

void ZoneBase::CreateLayer(int layer)
{
    mDrawables.insert(std::make_pair(layer, std::vector<IDrawablePtr>()));
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
