
#include "AnimationSequence.h"

using namespace mono;

AnimationSequence::AnimationSequence(const char* name, bool loop_sequence)
    : m_loop(loop_sequence),
      m_name(name)
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
    if(m_frames.size() == 1)
        return;

    m_frames.erase(m_frames.begin() + frame_number);
    Restart();
}

void AnimationSequence::Update(uint32_t delta_ms)
{
    if(m_done || m_frames.empty())
        return;
    
    m_elapsedTime += delta_ms;

    const mono::Frame& frame = m_frames.at(m_current_frame);
    if(m_elapsedTime > frame.duration)
    {
        m_elapsedTime = 0;
        m_current_frame++;

        if(m_current_frame >= m_frames.size())
        {
            m_current_frame = 0;
            if(!m_loop)
                m_done = true;
        }
    }
}

int AnimationSequence::Frame() const
{
    if(m_frames.empty())
        return 0;

    if(m_done)
        return m_frames.back().frame;

    return m_frames.at(m_current_frame).frame;
}

bool AnimationSequence::Finished() const
{
    return m_done;
}

void AnimationSequence::Restart()
{
    m_current_frame = 0;
    m_elapsedTime = 0;
    m_done = false;
}

bool AnimationSequence::IsLooping() const
{
    return m_loop;
}

void AnimationSequence::SetLooping(bool state)
{
    m_loop = state;
}

const char* AnimationSequence::GetName() const
{
    return m_name.c_str();
}

void AnimationSequence::SetName(const char* name)
{
    m_name = name;
}

const std::vector<mono::Frame>& AnimationSequence::GetFrames() const
{
    return m_frames;
}

std::vector<mono::Frame>& AnimationSequence::GetFrames()
{
    return m_frames;
}

