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
    
    mLayersDrawables[BACKGROUND]   = std::vector<IDrawablePtr>();
    mLayersDrawables[MIDDLEGROUND] = std::vector<IDrawablePtr>();
    mLayersDrawables[FOREGROUND]   = std::vector<IDrawablePtr>();
    
    mLayersEntities[BACKGROUND]   = std::vector<IEntityPtr>();
    mLayersEntities[MIDDLEGROUND] = std::vector<IEntityPtr>();
    mLayersEntities[FOREGROUND]   = std::vector<IEntityPtr>();
}

void ZoneBase::Accept(IRenderer& renderer)
{
    DoPreAccept();
    
    const auto addDrawableFunc = [&renderer](IDrawablePtr drawable) {
        renderer.AddDrawable(drawable);
    };
    
    const auto addEntityFunc = [&renderer](IEntityPtr entity) {
        renderer.AddDrawable(entity);
        renderer.AddUpdatable(entity);
    };
    
    const auto addUpdatableFunc = [&renderer](IUpdatablePtr updatable) {
        renderer.AddUpdatable(updatable);
    };
    
    std::for_each(mLayersDrawables[BACKGROUND].begin(), mLayersDrawables[BACKGROUND].end(), addDrawableFunc);
    std::for_each(mLayersEntities[BACKGROUND].begin(),  mLayersEntities[BACKGROUND].end(),  addEntityFunc);
    
    std::for_each(mLayersDrawables[MIDDLEGROUND].begin(), mLayersDrawables[MIDDLEGROUND].end(), addDrawableFunc);
    std::for_each(mLayersEntities[MIDDLEGROUND].begin(),  mLayersEntities[MIDDLEGROUND].end(),  addEntityFunc);

    std::for_each(mLayersDrawables[FOREGROUND].begin(), mLayersDrawables[FOREGROUND].end(), addDrawableFunc);
    std::for_each(mLayersEntities[FOREGROUND].begin(),  mLayersEntities[FOREGROUND].end(),  addEntityFunc);
    
    std::for_each(mUpdatables.begin(), mUpdatables.end(), addUpdatableFunc);
}

void ZoneBase::DoPreAccept()
{
    const auto removeIfDeadFunc = [](IEntityPtr entity) {
        return entity->RemoveMe();
    };
    
    for(auto& layer : mLayersEntities)
    {
        std::vector<IEntityPtr>& entities = layer.second;
        const auto removeIt = std::remove_if(entities.begin(), entities.end(), removeIfDeadFunc);
        if(removeIt != entities.end())
            entities.erase(removeIt, entities.end());
    }
}

void ZoneBase::AddEntity(const IEntityPtr& entity, int layer)
{
    auto layerIt = mLayersEntities.find(layer);
    if(layerIt == mLayersEntities.end())
        throw std::runtime_error("ZoneBase - Adding entity to missing layer");
    
    layerIt->second.push_back(entity);
}

void ZoneBase::RemoveEntity(const IEntityPtr& entity)
{
    bool result = false;
    for(auto& layer : mLayersEntities)
    {
        result = mono::FindAndRemove(layer.second, entity);
        if(result)
            break;
    }
    
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
    auto layerIt = mLayersDrawables.find(layer);
    if(layerIt == mLayersDrawables.end())
        throw std::runtime_error("ZoneBase - Adding drawable to missing layer");
    
    layerIt->second.push_back(drawable);
}

void ZoneBase::RemoveDrawable(const IDrawablePtr& drawable)
{
    bool result = false;
    for(auto& layer : mLayersDrawables)
    {
        result = mono::FindAndRemove(layer.second, drawable);
        if(result)
            break;
    }
    
    if(!result)
        throw std::runtime_error("ZoneBase - Unable to remove drawable");
}

void ZoneBase::ClearEntities()
{
    for(auto& drawables : mLayersDrawables)
        drawables.second.clear();
}



