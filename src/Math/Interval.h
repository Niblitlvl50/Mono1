
#pragma once

namespace math
{
    struct Interval
    {
        Interval()
        { }

        Interval(float min, float max)
            : min(min)
            , max(max)
        { }

        float min = 0.0f;
        float max = 0.0f;
    };

    float Interpolate(const Interval& interval, float t);
    float Size(const Interval& interval);


    struct ValueSpread
    {
        float value;
        Interval spread;
    };
}
