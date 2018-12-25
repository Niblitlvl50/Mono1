
#include "FpsCounter.h"
#include "System/System.h"
#include <cmath>

using namespace mono;

void FpsCounter::operator ++ (int)
{
    ++mFrames;
    ++mTotalFrames;
    
    const unsigned int currentTime = System::GetMilliseconds();
    const unsigned int delta = currentTime - mTime;
    if(delta >= 1000)
    {
        mTime = currentTime;
        mFps = (unsigned int)std::floor(float(mFrames) / 1.0f);
        mFrames = 0;
    }    
}

unsigned int FpsCounter::Fps() const
{
    return mFps;
}

unsigned int FpsCounter::Frames() const
{
    return mTotalFrames;
}


