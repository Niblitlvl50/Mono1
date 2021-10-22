
#include "Interval.h"

float math::Interpolate(const Interval& interval, float t)
{
    return (interval.max - interval.min) * t + interval.min;
}

float math::Size(const Interval& interval)
{
    return interval.max - interval.min;
}
