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

    for(auto& pair : mDrawables)
        renderer.AddDrawable(pair.second);

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
    mDrawables.push_back(std::make_pair(layer, drawable));

    // Keep the drawable vector sorted so that we draw everything
    // in the correct order according to layers

    const auto func = [](const std::pair<int, IDrawablePtr>& first,
                         const std::pair<int, IDrawablePtr>& second) {
        return first.first < second.first;
    };

    std::sort(mDrawables.begin(), mDrawables.end(), func);
}

void ZoneBase::RemoveDrawable(const IDrawablePtr& drawable)
{
    const auto func = [drawable](const std::pair<int, IDrawablePtr>& pair) {
        return pair.second == drawable;
    };

    auto it = std::find_if(mDrawables.begin(), mDrawables.end(), func);
    if(it == mDrawables.end())
        throw std::runtime_error("ZoneBase - Unable to remove drawable");

    mDrawables.erase(it);
}
