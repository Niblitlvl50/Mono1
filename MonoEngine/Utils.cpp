//
//  Utils.cpp
//  Mono1
//
//  Created by Niblit on 2012-05-21.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "Utils.h"
#include "SysTime.h"
#include <cmath>

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


