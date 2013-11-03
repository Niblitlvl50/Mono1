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

namespace
{
    struct AddEntityFunctor
    {
        AddEntityFunctor(IRenderer& renderer)
            : mRenderer(renderer)
        { }
        void operator () (IEntityPtr entity)
        {
            mRenderer.AddEntity(entity);
        }
        IRenderer& mRenderer;
    };
    
    struct AddUppdatableFunctor
    {
        AddUppdatableFunctor(IRenderer& renderer)
            : mRenderer(renderer)
        { }
        void operator () (IUpdatablePtr updatable)
        {
            mRenderer.AddUpdatable(updatable);
        }
        IRenderer& mRenderer;
    };
    

    bool RemoveIfDead(IEntityPtr entity)
    {
        return entity->RemoveMe();
    }
}

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
                   
    std::for_each(mLayers[BACKGROUND].begin(), mLayers[BACKGROUND].end(), AddEntityFunctor(renderer));
    std::for_each(mLayers[MIDDLEGROUND].begin(), mLayers[MIDDLEGROUND].end(), AddEntityFunctor(renderer));
    std::for_each(mLayers[FOREGROUND].begin(), mLayers[FOREGROUND].end(), AddEntityFunctor(renderer));
    
    std::for_each(mUpdatables.begin(), mUpdatables.end(), AddUppdatableFunctor(renderer));
}

void ZoneBase::DoPreAccept()
{
    for(auto it = mLayers.begin(), end = mLayers.end(); it != end; ++it)
    {
        std::vector<IEntityPtr>& collection = it->second;
        auto removeIt = std::remove_if(collection.begin(), collection.end(), RemoveIfDead);
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



