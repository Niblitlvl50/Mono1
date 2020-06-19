
#pragma once

#include "MathFwd.h"
#include <vector>
#include <cfloat>
#include <cmath>

namespace math
{    
    bool PointInsideQuad(const math::Vector& point, const math::Quad& quad);

    enum class LinePointResult
    {
        LEFT_OF,
        RIGHT_OF,
        ON_LINE
    };

    LinePointResult PointOnLine(const math::Vector& line_start, const math::Vector& line_end, const math::Vector& point);

    bool QuadOverlaps(const math::Quad& left, const math::Quad& right);
    void ResizeQuad(math::Quad& quad, float value, float aspect = 1.0f);
    math::Quad ResizeQuad(const math::Quad& quad, float value, float aspect = 1.0f);

    // Return angle in radians
    float AngleBetweenPoints(const math::Vector& first, const math::Vector& second);

    math::Vector VectorFromAngle(float radians);
    float AngleFromVector(const math::Vector& normal);
    float NormalizeAngle(float radians);

    // Pass in at least 3 points, else you will get "nan nan" back.
    math::Vector CentroidOfPolygon(const std::vector<math::Vector>& points);
    bool PointInsidePolygon(const math::Vector& point, const std::vector<math::Vector>& polygon);
    bool LineIntersectsPolygon(const math::Vector& start, const math::Vector& end, const std::vector<math::Vector>& polygon);

    // Check if a polygon is clockwise oriented or not
    bool IsPolygonClockwise(const std::vector<math::Vector>& points);

    math::Vector MapVectorInQuad(const math::Vector& point, const math::Quad& quad);
    math::Vector ClosestPointOnLine(const math::Vector& start, const math::Vector& end, const math::Vector& point);

    bool LineIntersectsLine(
        const math::Vector& start_first, const math::Vector& end_first,
        const math::Vector& start_second, const math::Vector& end_second);

    bool IsPrettyMuchEquals(float left, float right, float tolerance = FLT_EPSILON);

    inline float Align(float value, float size)
    {
        return std::floor(value / size) * size;
    }

    constexpr float PI() {
        return 3.14159265358979323846264338327950288f;
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
