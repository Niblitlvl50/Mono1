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
        const bool result = FindAndRemoveEntity(it->second, entity);
        if(result)
            return;
    }
}
