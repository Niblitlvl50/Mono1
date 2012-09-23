//
//  Utils.h
//  Mono1
//
//  Created by Niblit on 2012-05-21.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#pragma once

#include "MonoFwd.h"

namespace mono
{    
    bool FindAndRemoveEntity(IEntityCollection& collection, IEntityPtr entity);

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
