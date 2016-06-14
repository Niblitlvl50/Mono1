//
//  Utils.h
//  Mono1
//
//  Created by Niblit on 2012-05-21.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#pragma once

#include "SysTypes.h"
#include <vector>

namespace mono
{    
    template <typename T>
    inline bool FindAndRemove(std::vector<std::shared_ptr<T>>& collection, const std::shared_ptr<T>& object)
    {
        const auto it = std::find(collection.begin(), collection.end(), object);
        if(it != collection.end())
        {
            collection.erase(it);
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

    //! Generates a random float between 0.0 and 1.0
    float Random();
}
