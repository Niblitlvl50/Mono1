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
    
    mLayers[BACKGROUND]   = std::vector<IEntityPtr>();
    mLayers[MIDDLEGROUND] = std::vector<IEntityPtr>();
    mLayers[FOREGROUND]   = std::vector<IEntityPtr>();
}

void ZoneBase::Accept(IRenderer& renderer)
{
    DoPreAccept();
    
    const auto addEntityFunc = [&renderer](IEntityPtr entity) {
        renderer.AddEntity(entity);
    };
    
    const auto addUpdatableFunc = [&renderer](IUpdatablePtr updatable) {
        renderer.AddUpdatable(updatable);
    };
    
    std::for_each(mLayers[BACKGROUND].begin(), mLayers[BACKGROUND].end(), addEntityFunc);
    std::for_each(mLayers[MIDDLEGROUND].begin(), mLayers[MIDDLEGROUND].end(), addEntityFunc);
    std::for_each(mLayers[FOREGROUND].begin(), mLayers[FOREGROUND].end(), addEntityFunc);
    
    std::for_each(mUpdatables.begin(), mUpdatables.end(), addUpdatableFunc);
}

void ZoneBase::DoPreAccept()
{
    const auto removeIfDeadFunc = [](IEntityPtr entity) {
        return entity->RemoveMe();
    };
    
    for(auto it = mLayers.begin(), end = mLayers.end(); it != end; ++it)
    {
        std::vector<IEntityPtr>& collection = it->second;
        auto removeIt = std::remove_if(collection.begin(), collection.end(), removeIfDeadFunc);
        if(removeIt != collection.end())
            collection.erase(removeIt, collection.end());
    }    
}

void ZoneBase::AddEntityToLayer(IEntityPtr entity, LayerId layer)
{
    auto layerIt = mLayers.find(layer);
    std::vector<IEntityPtr>& collection = layerIt->second;
    collection.push_back(entity);
}

void ZoneBase::RemoveEntity(IEntityPtr entity)
{
    bool result = false;
    for(auto it = mLayers.begin(), end = mLayers.end(); it != end; ++it)
    {
        result = FindAndRemove(it->second, entity);
        if(result)
            break;
    }
    
    if(!result)
        throw std::runtime_error("Unable to remove entity");
}

void ZoneBase::AddUpdatable(IUpdatablePtr updatable)
{
    mUpdatables.push_back(updatable);
}

void ZoneBase::RemoveUpdatable(IUpdatablePtr updatable)
{
    const bool result = mono::FindAndRemove(mUpdatables, updatable);
    if(!result)
        throw std::runtime_error("Unable to remove updatable");
}

void ZoneBase::ClearEntities()
{
    for(auto& entities : mLayers)
        entities.second.clear();
}



