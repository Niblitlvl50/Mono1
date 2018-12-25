
#pragma once

namespace mono
{
    class FpsCounter
    {
    public:

        void operator ++ (int i);
        unsigned int Fps() const;
        unsigned int Frames() const;

    private:

        unsigned int mTime = 0;
        unsigned int mFrames = 0;
        unsigned int mTotalFrames = 0;
        unsigned int mFps = 0;
    };
}
