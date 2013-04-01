//
//  AnimationSequence.cpp
//  Mono1
//
//  Created by Niblit on 2012-04-20.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "AnimationSequence.h"

using namespace mono;

AnimationSequence::AnimationSequence(unsigned int start, unsigned int end, bool loop, const FrameDurations& durations)
    : mStart(start),
      mEnd(end),
      mFrame(start),
      mElapsedTime(0),
      mLoopSequence(loop),
      mDone(false),
      mDurations(durations)
{ }

void AnimationSequence::Tick(unsigned int delta)
{
    if(mDone)
        return;
    
    mElapsedTime += delta;
    if(mElapsedTime > mDurations.at(mFrame - mStart))
    {
        mElapsedTime = 0;
        mFrame++;
        
        if(mFrame > mEnd)
        {
            if(mLoopSequence)
                mFrame = mStart;
            else
                mDone = true;
        }
    }
}

unsigned int AnimationSequence::Frame() const
{
    return mFrame;
}

bool AnimationSequence::Finished() const
{
    return mDone;
}


