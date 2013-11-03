//
//  Utils.h
//  Mono1
//
//  Created by Niblit on 2012-05-21.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#pragma once

#include "MonoFwd.h"
#include <vector>

namespace
{
    template <typename T>
    struct ObjectFinder
    {
        ObjectFinder(const std::shared_ptr<T> object)
            : mObject(object)
        { }
        bool operator()(const std::shared_ptr<T> other) const
        {
            return mObject == other;
        }
        const std::shared_ptr<T> mObject;
    };
}

namespace mono
{    
    bool FindAndRemoveEntity(std::vector<IEntityPtr>& collection, IEntityPtr entity);
    
    template <typename T>
    bool FindAndRemove(std::vector<std::shared_ptr<T> >& collection, std::shared_ptr<T> object)
    {
        const ObjectFinder<T> finder(object);
        const auto newEnd = std::remove_if(collection.begin(), collection.end(), finder);
        if(newEnd != collection.end())
        {
            collection.erase(newEnd);
            return true;
        }
        
        return false;
    }

    class FPSCounter
    {
    public:
        
        FPSCounter();

        void operator ++ (int i);
        
        unsigned int Fps() const;
        unsigned int Frames() const;
        
    private:
        
        unsigned int mTime;
        unsigned int mFrames;
        unsigned int mTotalFrames;
        unsigned int mFps;
    };    
}
