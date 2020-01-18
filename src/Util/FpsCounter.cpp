
#include "FpsCounter.h"
#include "System/System.h"
#include <cmath>

using namespace mono;

void FpsCounter::operator ++ (int)
{
    ++m_frames;
    ++m_total_frames;
    
    const uint32_t current_time = System::GetMilliseconds();
    const uint32_t delta = current_time - m_time;
    if(delta >= 1000)
    {
        m_time = current_time;
        m_fps = (uint32_t)std::floor(float(m_frames) / 1.0f);
        m_frames = 0;
    }
}

uint32_t FpsCounter::Fps() const
{
    return m_fps;
}

uint32_t FpsCounter::Frames() const
{
    return m_total_frames;
}


