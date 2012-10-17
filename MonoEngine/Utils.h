//
//  Utils.h
//  Mono1
//
//  Created by Niblit on 2012-05-21.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#pragma once

#include "MonoFwd.h"

namespace
{
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
}

namespace mono
{    
    bool FindAndRemoveEntity(IEntityCollection& collection, IEntityPtr entity);
    
    template <typename T>
    bool FindAndRemove(std::vector<std::tr1::shared_ptr<T> >& collection, std::tr1::shared_ptr<T> object)
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

    class FPSCounter
    {
    public:
        
        FPSCounter();

        void operator ++ (int i);
        
        unsigned int fps() const;
        unsigned int frames() const;
        
    private:
        
        unsigned int mTime;
        unsigned int mFrames;
        unsigned int mTotalFrames;
        unsigned int mFps;
    };    
}
