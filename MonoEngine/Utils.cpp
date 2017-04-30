
#include "Utils.h"
#include "System/System.h"
#include <cmath>

using namespace mono;

FPSCounter::FPSCounter()
    : mTime(0),
      mFrames(0),
      mTotalFrames(0),
      mFps(0)
{ }

void FPSCounter::operator ++ (int)
{
    ++mFrames;
    ++mTotalFrames;
    
    const unsigned int currentTime = System2::GetMilliseconds();
    const unsigned int delta = currentTime - mTime;
    if(delta >= 1000)
    {
        mTime = currentTime;
        mFps = (unsigned int)std::floor(float(mFrames) / 1.0f);
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


