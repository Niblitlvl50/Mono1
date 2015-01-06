//
//  AnimationSequence.h
//  Mono1
//
//  Created by Niblit on 2012-04-20.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//


#pragma once

#include <vector>

namespace mono
{
    typedef std::vector<unsigned int> FrameDurations;

    struct AnimationSequence
    {
        AnimationSequence(unsigned int start, unsigned int end, bool loop, const FrameDurations& durations);        

        void Tick(unsigned int delta);
        unsigned int Frame() const;
        bool Finished() const;
        
    private:
        
        unsigned int mStart;
        unsigned int mEnd;
        unsigned int mFrame;
        unsigned int mElapsedTime;
        bool mLoopSequence;
        bool mDone;
        FrameDurations mDurations;
    };
}
