
#include "Bezier.h"

math::Vector math::Cubic(float t, const math::Vector* points)
{
    const float u = 1.0f - t;
    float tt = t * t;
    float uu = u * u;
    float uuu = uu * u;
    float ttt = tt * t;

    math::Vector p = points[0] * uuu; 
    p += points[1] * 3 * uu * t;
    p += points[2] * 3 * u * tt;
    p += points[3] * ttt;

    return p;
}
