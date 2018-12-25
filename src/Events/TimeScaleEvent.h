
#pragma once

namespace event
{
    struct TimeScaleEvent
    {
        constexpr TimeScaleEvent(float scale)
            : time_scale(scale)
        { }

        const float time_scale;
    };
}