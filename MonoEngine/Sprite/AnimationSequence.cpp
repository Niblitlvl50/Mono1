//
//  AnimationSequence.cpp
//  Mono1
//
//  Created by Niblit on 2012-04-20.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "AnimationSequence.h"

using namespace mono;

AnimationSequence::AnimationSequence()
    : mStart(0),
      mEnd(0),
      mFrame(0),
      mElapsedTime(0)
{ }

AnimationSequence::AnimationSequence(unsigned int start, unsigned int end, const FrameDurations& durations)
    : mStart(start),
      mEnd(end),
      mFrame(start),
      mElapsedTime(0),
      mDurations(durations)
{ }

void AnimationSequence::Tick(unsigned int delta)
{
    mElapsedTime += delta;
    if(mElapsedTime > mDurations.at(mFrame - mStart))
    {
        mElapsedTime = 0;
        mFrame++;
        
        if(mFrame > mEnd)
            mFrame = mStart;
    }
}

unsigned int AnimationSequence::Frame() const
{
    return mFrame;
}

