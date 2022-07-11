
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
    bool Validate(const Interval& interval);


    struct ValueSpread
    {
        ValueSpread()
            : value(0.0f)
            , spread(0.0f, 0.0f)
        { }

        ValueSpread(float value, float min, float max)
            : value(value)
            , spread(min, max)
        { }

        float value;
        Interval spread;
    };

    inline bool Validate(const ValueSpread& value_spread)
    {
        return Validate(value_spread.spread);
    }
}
