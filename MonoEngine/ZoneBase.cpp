//
//  ZoneBase.cpp
//  Mono1
//
//  Created by Niblit on 2012-04-21.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "ZoneBase.h"
#include "IRenderer.h"
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
}

ZoneBase::ZoneBase()
{
    // Make sure the three layers exists.
    
    mLayers[BACKGROUND] = IEntityCollection();
    mLayers[MIDDLEGROUND] = IEntityCollection();
    mLayers[FOREGROUND] = IEntityCollection();
}

void ZoneBase::Accept(IRenderer& renderer)
{    
    std::for_each(mLayers[BACKGROUND].begin(), mLayers[BACKGROUND].end(), AddEntityFunctor(renderer));
    std::for_each(mLayers[MIDDLEGROUND].begin(), mLayers[MIDDLEGROUND].end(), AddEntityFunctor(renderer));
    std::for_each(mLayers[FOREGROUND].begin(), mLayers[FOREGROUND].end(), AddEntityFunctor(renderer));
    
    std::for_each(mUpdatables.begin(), mUpdatables.end(), AddUppdatableFunctor(renderer));
}

void ZoneBase::AddEntityToLayer(IEntityPtr entity, LayerId layer)
{
    LayerMap::iterator layerIt = mLayers.find(layer);    
    IEntityCollection& collection = layerIt->second;
    collection.push_back(entity);
}

void ZoneBase::RemoveEntity(IEntityPtr entity)
{
    bool result = false;
    for(LayerMap::iterator it = mLayers.begin(), end = mLayers.end(); it != end; ++it)
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



