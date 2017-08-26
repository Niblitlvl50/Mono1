
#pragma once

#include "MathFwd.h"
#include <vector>

namespace math
{    
    bool PointInsideQuad(const math::Vector& point, const math::Quad& quad);
    bool QuadOverlaps(const math::Quad& left, const math::Quad& right);
    void ResizeQuad(math::Quad& quad, float value, float aspect = 1.0f);

    // Return angle in radians
    float AngleBetweenPoints(const math::Vector& first, const math::Vector& second);

    math::Vector VectorFromAngle(float radians);

    // Pass in at least 3 points, else you will get "nan nan" back.
    math::Vector CentroidOfPolygon(const std::vector<math::Vector>& points);
    bool PointInsidePolygon(const math::Vector& point, const std::vector<math::Vector>& polygon);

    // Check if a polygon is clockwise oriented or not
    bool IsPolygonClockwise(const std::vector<math::Vector>& points);

    math::Vector MapVectorInQuad(const math::Vector& point, const math::Quad& quad);
    math::Vector ClosestPointOnLine(const math::Vector& start, const math::Vector& end, const math::Vector& point);

    constexpr float PI() {
        return 3.14159265359f;
    }

    constexpr float PI_2() {
        return PI() / 2.0f;
    }

    constexpr float PI_4() {
        return PI() / 4.0f;
    }

    constexpr float ToRadians(float degrees) {
        return degrees * PI() / 180.0f;
    }

    constexpr float ToDegrees(float radians) {
        return radians * 180.0f / PI();
    }
}
