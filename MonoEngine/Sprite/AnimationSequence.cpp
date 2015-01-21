//
//  AnimationSequence.cpp
//  Mono1
//
//  Created by Niblit on 2012-04-20.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "AnimationSequence.h"

using namespace mono;

AnimationSequence::AnimationSequence(bool loopSequence)
    : mCurrentFrame(0),
      mElapsedTime(0),
      mLoopSequence(loopSequence),
      mDone(false)
{ }

void AnimationSequence::AddFrame(unsigned int frameNumber, unsigned int duration)
{
    mFrames.push_back(mono::Frame(frameNumber, duration));
}

void AnimationSequence::Tick(unsigned int delta)
{
    if(mDone)
        return;
    
    mElapsedTime += delta;

    const mono::Frame& frame = mFrames.at(mCurrentFrame);
    if(mElapsedTime > frame.duration)
    {
        mElapsedTime = 0;
        mCurrentFrame++;

        if(mCurrentFrame >= mFrames.size())
        {
            mCurrentFrame = 0;
            if(!mLoopSequence)
                mDone = true;
        }
    }
}

unsigned int AnimationSequence::Frame() const
{
    return mFrames.at(mCurrentFrame).frame;
}

bool AnimationSequence::Finished() const
{
    return mDone;
}


