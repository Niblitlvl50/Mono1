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
        int frame = 0;
        int duration = 0;
    };

    class AnimationSequence
    {
    public:

        AnimationSequence(bool loop_sequence);

        void AddFrame(int frame_number, int duration);
        void RemoveFrame(int frame_number);

        void Update(unsigned int delta);

        int Frame() const;
        bool Finished() const;
        void Restart();

        bool IsLooping() const;
        void SetLooping(bool state);

        const std::vector<mono::Frame>& GetFrames() const;
              std::vector<mono::Frame>& GetFrames();

    private:

        unsigned int m_currentFrame;
        int m_elapsedTime;

        bool m_loopSequence;
        bool m_done;

        std::vector<mono::Frame> m_frames;
    };
}
