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
    void RenderLayer(const IEntityCollection& collection, mono::IRenderer& renderer)
    {
        for(IEntityCollection::const_iterator it = collection.begin(), end = collection.end(); it != end; ++it)
        {
            mono::IEntityPtr entity = *it;
            renderer.AddEntity(entity);            
        }
    }
    
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
    RenderLayer(mLayers[BACKGROUND], renderer);
    RenderLayer(mLayers[MIDDLEGROUND], renderer);
    RenderLayer(mLayers[FOREGROUND], renderer);
    
    std::for_each(mUpdatables.begin(), mUpdatables.end(), AddUppdatableFunctor(renderer));
}

void ZoneBase::AddEntityToLayer(LayerId layer, IEntityPtr entity)
{
    LayerMap::iterator layerIt = mLayers.find(layer);    
    IEntityCollection& collection = layerIt->second;
    collection.push_back(entity);
}

void ZoneBase::RemoveEntity(IEntityPtr entity)
{
    for(LayerMap::iterator it = mLayers.begin(), end = mLayers.end(); it != end; ++it)
    {
        const bool result = FindAndRemove(it->second, entity);
        if(result)
            return;
    }
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



