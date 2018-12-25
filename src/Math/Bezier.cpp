
#include "Bezier.h"
#include "MathFwd.h"
#include <vector>

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

/*
float closestPoint(const std::vector<math::Vector>& points, const math::Vector& point)
{
    float pathLength = pathNode.getTotalLength();
    float precision = 8.0f;
    float best;
    float bestLength;
    float bestDistance = math::INF;

    const auto distance2 = [](const math::Vector& p) {
        float dx = p.x - point[0]:
        float dy = p.y - point[1];
        return dx * dx + dy * dy;
    }

    // linear scan for coarse approximation
    for (var scan, scanLength = 0, scanDistance; scanLength <= pathLength; scanLength += precision)
    {
        if ((scanDistance = distance2(scan = pathNode.getPointAtLength(scanLength))) < bestDistance)
        {
            best = scan, bestLength = scanLength, bestDistance = scanDistance;
        }
    }

    // binary search for precise estimate
    precision /= 2;
    while (precision > 0.5)
    {
        var before,
            after,
            beforeLength,
            afterLength,
            beforeDistance,
            afterDistance;
        if ((beforeLength = bestLength - precision) >= 0 && (beforeDistance = distance2(before = pathNode.getPointAtLength(beforeLength))) < bestDistance) {
            best = before, bestLength = beforeLength, bestDistance = beforeDistance;
        } else if ((afterLength = bestLength + precision) <= pathLength && (afterDistance = distance2(after = pathNode.getPointAtLength(afterLength))) < bestDistance) {
            best = after, bestLength = afterLength, bestDistance = afterDistance;
        } else {
            precision /= 2;
        }
    }

    best = [best.x, best.y];
    best.distance = Math.sqrt(bestDistance);
    return best;
}
*/