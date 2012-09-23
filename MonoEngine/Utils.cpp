//
//  Utils.cpp
//  Mono1
//
//  Created by Niblit on 2012-05-21.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "Utils.h"
#include "IEntity.h"
#include "SysTime.h"
#include <cmath>

namespace
{
    struct EntityFinder
    {
        EntityFinder(const mono::IEntityPtr entity)
            : mEntity(entity)
        { }
        bool operator()(const mono::IEntityPtr other) const
        {
            return mEntity == other;
        }
        const mono::IEntityPtr mEntity;
    };
}

bool mono::FindAndRemoveEntity(IEntityCollection& collection, IEntityPtr entity)
{
    const EntityFinder finder(entity);
    const IEntityCollection::iterator newEnd = std::remove_if(collection.begin(), collection.end(), finder);
    if(newEnd != collection.end())
    {
        collection.erase(newEnd, collection.end());
        return true;
    }
    
    return false;
}

using namespace mono;

FPSCounter::FPSCounter()
    : mTime(0),
      mFrames(0),
      mTotalFrames(0),
      mFps(0)
{ }

void FPSCounter::operator ++ (int i)
{
    ++mFrames;
    ++mTotalFrames;
    
    const unsigned int currentTime = Time::GetMilliseconds();
    const unsigned int delta = currentTime - mTime;
    if(delta >= 1000)
    {
        mTime = currentTime;
        mFps = std::floor(float(mFrames) / 1.0f);
        mFrames = 0;
    }    
}

unsigned int FPSCounter::fps() const
{
    return mFps;
}

unsigned int FPSCounter::frames() const
{
    return mTotalFrames;
}


