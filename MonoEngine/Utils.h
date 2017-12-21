
#pragma once

#include <algorithm>
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
