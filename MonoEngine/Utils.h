
#pragma once

#include "System/SysTypes.h"
#include <vector>

namespace mono
{
    template <typename T>
    inline bool FindAndRemove(std::vector<T>& collection, const T& object)
    {
        const auto it = std::find(collection.begin(), collection.end(), object);
        if(it != collection.end())
        {
            collection.erase(it);
            return true;
        }

        return false;
    }

    class FPSCounter
    {
    public:

        FPSCounter();

        void operator ++ (int i);

        unsigned int Fps() const;
        unsigned int Frames() const;

    private:

        unsigned int mTime;
        unsigned int mFrames;
        unsigned int mTotalFrames;
        unsigned int mFps;
    };
}
