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
#include "Math/Quad.h"
#include "Math/MathFunctions.h"

#include <cstdio>
//#include <stdexcept>

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
    for(const auto& task : m_preFrameTasks)
        task();

    m_preFrameTasks.clear();
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
        std::printf("ZoneBase - Unable to remove entity with id %u", entity->Id());
        //throw std::runtime_error("ZoneBase - Unable to remove entity");
}

void ZoneBase::AddUpdatable(const IUpdatablePtr& updatable)
{
    mUpdatables.push_back(updatable);
}

void ZoneBase::RemoveUpdatable(const IUpdatablePtr& updatable)
{
    const bool result = mono::FindAndRemove(mUpdatables, updatable);
    if(!result)
        std::printf("ZoneBase - Unable to remove updatable");
        //throw std::runtime_error("ZoneBase - Unable to remove updatable");
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
        std::printf("ZoneBase - Unable to remove drawable");
        //throw std::runtime_error("ZoneBase - Unable to remove drawable");

    mDrawables.erase(it);
}

mono::IEntityPtr ZoneBase::FindEntityFromId(uint id) const
{
    const auto find_func = [id](const mono::IEntityPtr& entity) {
        return id == entity->Id();
    };

    const auto& it = std::find_if(mEntities.begin(), mEntities.end(), find_func);
    if(it != mEntities.end())
        return *it;

    return nullptr;
}

mono::IEntityPtr ZoneBase::FindEntityFromPoint(const math::Vector2f& point) const
{
    const auto find_func = [&point](const mono::IEntityPtr& entity) {
        const math::Quad& bb = entity->BoundingBox();
        return math::PointInsideQuad(point, bb);
    };

    const auto& it = std::find_if(mEntities.begin(), mEntities.end(), find_func);
    if(it != mEntities.end())
        return *it;

    return nullptr;
}

void ZoneBase::SchedulePreFrameTask(const std::function<void ()>& task)
{
    m_preFrameTasks.push_back(task);
}

