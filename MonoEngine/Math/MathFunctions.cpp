
#include "MathFunctions.h"
#include "Vector.h"
#include "Quad.h"

#include <cmath>
#include <stdlib.h>

bool math::PointInsideQuad(const math::Vector& point, const math::Quad& quad)
{
    if(point.x > quad.mA.x &&
       point.x < quad.mB.x &&
       point.y > quad.mA.y &&
       point.y < quad.mB.y)
        return true;
    
    return false;
}

math::LinePointResult math::PointOnLine(const math::Vector& line_start, const math::Vector& line_end, const math::Vector& point)
{
    const int result =
        (line_end.x - line_start.x) * (point.y - line_start.y) - (point.x - line_start.x) * (line_end.y - line_start.y);
    
    if(result > 0)
        return LinePointResult::LEFT_OF;
    else if(result < 0)
        return LinePointResult::RIGHT_OF;
    else
        return LinePointResult::ON_LINE;
}

bool math::QuadOverlaps(const math::Quad& left, const math::Quad& right)
{
    const Vector& left1 = left.mA;
    const Vector& left2 = left.mB;
    
    const Vector& right1 = right.mA;
    const Vector& right2 = right.mB;
    
    if( left1.x < right2.x && left2.x > right1.x &&
        left1.y < right2.y && left2.y > right1.y )
        return true;
    
    return false;
}

void math::ResizeQuad(math::Quad& quad, float value, float aspect)
{
    const float resizeX = value * aspect;
    const float resizeY = value;
    
    quad.mA.x -= (resizeX * 0.5f);
    quad.mA.y -= (resizeY * 0.5f);

    quad.mB.x += resizeX;
    quad.mB.y += resizeY;
}

float math::AngleBetweenPoints(const math::Vector& first, const math::Vector& second)
{
    return std::atan2(second.y - first.y, second.x - first.x);
}

math::Vector math::VectorFromAngle(float radians)
{
    return math::Vector(-std::sin(radians), std::cos(radians));
}

float math::AngleFromVector(const math::Vector& normal)
{
    return atan2f(normal.x, normal.y);
}

math::Vector math::CentroidOfPolygon(const std::vector<math::Vector>& points)
{
    math::Vector centroid;
    float area = 0.0f;

    const auto func = [&centroid, &area](const math::Vector& first, const math::Vector& second) {
        const float partial_area = first.x * second.y - second.x * first.y;

        area += partial_area;

        centroid.x += (first.x + second.x) * partial_area;
        centroid.y += (first.y + second.y) * partial_area;
    };


    for(size_t point_index = 0; point_index < points.size() -1; ++point_index)
        func(points.at(point_index), points.at(point_index +1));

    func(points.back(), points.front());

    area *= 0.5;
    centroid.x /= (6.0 * area);
    centroid.y /= (6.0 * area);

    return centroid;
}

bool math::PointInsidePolygon(const math::Vector& point, const std::vector<math::Vector>& polygon)
{
    bool inside = false;

    for(size_t i = 0, j = polygon.size() -1; i < polygon.size(); j = i++)
    {
        const math::Vector& i_point = polygon[i];
        const math::Vector& j_point = polygon[j];

        const bool test1 = (i_point.y > point.y) != (j_point.y > point.y);
        const bool test2 = (point.x < (j_point.x - i_point.x) * (point.y - i_point.y) / (j_point.y - i_point.y) + i_point.x);

        if(test1 && test2)
            inside = !inside;
    }

    return inside;
}

bool math::LineIntersectsPolygon(const math::Vector& start, const math::Vector& end, const std::vector<math::Vector>& points)
{
    for(size_t point_index = 0; point_index < points.size(); ++point_index)
    {
        const math::Vector& v1 = points[point_index];
        const math::Vector& v2 = points[(point_index + 1) % points.size()];

        const bool intersects = LineIntersectsLine(start, end, v1, v2);
        if(intersects)
            return true;
    }

    return false;
}

bool math::IsPolygonClockwise(const std::vector<math::Vector>& points)
{
    float sum = 0.0f;

    for(size_t point_index = 0; point_index < points.size(); ++point_index)
    {
        const math::Vector& v1 = points[point_index];
        const math::Vector& v2 = points[(point_index + 1) % points.size()];

        sum += (v2.x - v1.x) * (v2.y + v1.y);
    }

    return sum > 0.0f;
}

math::Vector math::MapVectorInQuad(const math::Vector& point, const math::Quad& quad)
{
    const math::Vector& temp = point - quad.mA;
    const math::Vector& size = quad.mB - quad.mA;

    // We need to flip the y axis
    return math::Vector(temp.x / size.x, (size.y - temp.y) / size.y);
}

math::Vector math::ClosestPointOnLine(const math::Vector& start, const math::Vector& end, const math::Vector& point)
{
    const float length = math::LengthSquared(start - end);
    if(length == 0.0f)
        return start;

    // Consider the line extending the segment, parameterized as v + t (w - v).
    // We find projection of point p onto the line. 
    // It falls where t = [(p-v) . (w-v)] / |w-v|^2
    const float t = math::Dot((point - start), (end - start)) / length;

    if(t <= 0.0f)
        return start; // Beyond the 'v' end of the segment
    else if(t >= 1.0f)
        return end;   // Beyond the 'w' end of the segment

    // Projection falls on the segment
    const math::Vector& projection = start + (end - start) * t;
    return projection;
}

float math::NormalizeAngle(float radians)
{
    radians = fmod(radians, math::PI() * 2.0f);
    if(radians < 0.0f)
        radians += math::PI() * 2.0f;
    return radians;
}

bool math::LineIntersectsLine(
    const math::Vector& start_first, const math::Vector& end_first,
    const math::Vector& start_second, const math::Vector& end_second)
{
//inline bool lines_intersect_2d(Vector2 const& p0, Vector2 const& p1, Vector2 const& p2, Vector2 const& p3, Vector2* i const = 0) {
    const math::Vector& s1 = end_first - start_first;
    const math::Vector& s2 = end_second - start_second;

    const math::Vector& u = start_first - start_second;

    const float ip = 1.0f / (-s2.x * s1.y + s1.x * s2.y);

    const float s = (-s1.y * u.x + s1.x * u.y) * ip;
    const float t = ( s2.x * u.y - s2.y * u.x) * ip;

    if(s >= 0.0f && s <= 1.0f && t >= 0.0f && t <= 1.0f)
    {
//        if(i)
//            *i = p0 + (s1 * t);
        return true;
    }

    return false;
}
