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
    struct Frame
    {
        Frame()
            : frame(0),
              duration(-1)
        { }

        Frame(unsigned int frame, unsigned int duration)
            : frame(frame),
              duration(duration)
        { }

        unsigned int frame;
        unsigned int duration;
    };

    class AnimationSequence
    {
    public:

        AnimationSequence(bool loopSequence);

        void AddFrame(unsigned int frameNumber, unsigned int duration);
        void Tick(unsigned int delta);

        unsigned int Frame() const;
        bool Finished() const;

    private:

        unsigned int mCurrentFrame;
        unsigned int mElapsedTime;

        bool mLoopSequence;
        bool mDone;

        std::vector<mono::Frame> mFrames;
    };
}
