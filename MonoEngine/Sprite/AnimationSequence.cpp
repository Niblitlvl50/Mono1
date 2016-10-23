//
//  AnimationSequence.cpp
//  Mono1
//
//  Created by Niblit on 2012-04-20.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "AnimationSequence.h"

using namespace mono;

AnimationSequence::AnimationSequence(bool loop_sequence)
    : m_currentFrame(0),
      m_elapsedTime(0),
      m_loopSequence(loop_sequence),
      m_done(false)
{ }

void AnimationSequence::AddFrame(int frame_number, int duration)
{
    mono::Frame frame;
    frame.frame = frame_number;
    frame.duration = duration;
    
    m_frames.push_back(frame);
}

void AnimationSequence::RemoveFrame(int frame_number)
{
    m_frames.erase(m_frames.begin() + frame_number);
}

void AnimationSequence::Update(unsigned int delta)
{
    if(m_done)
        return;
    
    m_elapsedTime += delta;

    const mono::Frame& frame = m_frames.at(m_currentFrame);
    if(m_elapsedTime > frame.duration)
    {
        m_elapsedTime = 0;
        m_currentFrame++;

        if(m_currentFrame >= m_frames.size())
        {
            m_currentFrame = 0;
            if(!m_loopSequence)
                m_done = true;
        }
    }
}

int AnimationSequence::Frame() const
{
    return m_frames.at(m_currentFrame).frame;
}

bool AnimationSequence::Finished() const
{
    return m_done;
}

void AnimationSequence::Restart()
{
    m_currentFrame = 0;
    m_elapsedTime = 0;
    m_done = false;
}

bool AnimationSequence::IsLooping() const
{
    return m_loopSequence;
}

void AnimationSequence::SetLooping(bool state)
{
    m_loopSequence = state;
}

const std::vector<mono::Frame>& AnimationSequence::GetFrames() const
{
    return m_frames;
}

std::vector<mono::Frame>& AnimationSequence::GetFrames()
{
    return m_frames;
}

