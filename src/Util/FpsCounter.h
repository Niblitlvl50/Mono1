
#pragma once

#include <cstdint>

namespace mono
{
    class FpsCounter
    {
    public:

        void operator ++ (int i);
        uint32_t Fps() const;
        uint32_t Frames() const;
        float Delta() const;

    private:

        uint32_t m_time = 0;
        uint32_t m_frames = 0;
        uint32_t m_total_frames = 0;
        uint32_t m_fps = 0;
        float m_delta = 0.0f;
    };
}
