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
    
    /*
    template <typename T>
    struct ObjectFinder
    {
        ObjectFinder(const std::tr1::shared_ptr<T> object)
            : mObject(object)
        { }
        bool operator()(const std::tr1::shared_ptr<T> other) const
        {
            return mObject == other;
        }
        const std::tr1::shared_ptr<T> mObject;
    };
     */
}

bool mono::FindAndRemoveEntity(std::vector<IEntityPtr>& collection, IEntityPtr entity)
{
    const EntityFinder finder(entity);
    const auto newEnd = std::remove_if(collection.begin(), collection.end(), finder);
    if(newEnd != collection.end())
    {
        collection.erase(newEnd, collection.end());
        return true;
    }
    
    return false;
}

/*
template <typename T>
bool mono::FindAndRemove(std::vector<std::tr1::shared_ptr<T> >& collection, std::tr1::shared_ptr<T> object)
{
    typedef typename std::vector<std::tr1::shared_ptr<T> >::iterator TypeIterator;
    const ObjectFinder<T> finder(object);
    const TypeIterator newEnd = std::remove_if(collection.begin(), collection.end(), finder);
    if(newEnd != collection.end())
    {
        collection.erase(newEnd);
        return true;
    }
    
    return false;
}
 */


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

unsigned int FPSCounter::Fps() const
{
    return mFps;
}

unsigned int FPSCounter::Frames() const
{
    return mTotalFrames;
}


