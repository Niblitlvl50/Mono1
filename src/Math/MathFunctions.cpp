
#include "MathFunctions.h"
#include "Vector.h"
#include "Quad.h"

#include <cmath>
#include <stdlib.h>

bool math::PointInsideQuad(const math::Vector& point, const math::Quad& quad)
{
    if(point.x > quad.bottom_left.x &&
       point.x < quad.top_right.x &&
       point.y > quad.bottom_left.y &&
       point.y < quad.top_right.y)
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

math::PointOnLineResult math::ClosestPointOnLine(const math::Vector& start, const math::Vector& end, const math::Vector& point)
{
    PointOnLineResult result;
    result.point = start;
    result.t = 0.0f;

    const float length = math::LengthSquared(start - end);
    if(length == 0.0f)
        return result;

    // Consider the line extending the segment, parameterized as v + t (w - v).
    // We find projection of point p onto the line. 
    // It falls where t = [(p-v) . (w-v)] / |w-v|^2
    result.t = math::Dot((point - start), (end - start)) / length;

    if(result.t <= 0.0f)
    {
        // Beyond the 'v' end of the segment
        result.point = start;
    }
    else if(result.t >= 1.0f)
    {
        // Beyond the 'w' end of the segment
        result.point = end;
    }
    else
    {
        // Projection falls on the segment
        const math::Vector& projection = start + (end - start) * result.t;
        result.point = projection;
    }

    return result;
}

bool math::QuadOverlaps(const math::Quad& left, const math::Quad& right)
{
    const Vector& left1 = left.bottom_left;
    const Vector& left2 = left.top_right;
    
    const Vector& right1 = right.bottom_left;
    const Vector& right2 = right.top_right;
    
    if( left1.x < right2.x && left2.x > right1.x &&
        left1.y < right2.y && left2.y > right1.y )
        return true;
    
    return false;
}

void math::ResizeQuad(math::Quad& quad, float value, float aspect)
{
    const float resize_x = value * aspect;
    const float resize_y = value;
    
    quad.bottom_left.x -= resize_x;
    quad.bottom_left.y -= resize_y;

    quad.top_right.x += resize_x;
    quad.top_right.y += resize_y;
}

math::Quad math::ResizeQuad(const math::Quad& quad, float value, float aspect)
{
    math::Quad out_quad = quad;
    ResizeQuad(out_quad, value, aspect);
    return out_quad;
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
    return std::atan2(-normal.x, normal.y);
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

        const math::LineIntersectionResult result = LineIntersectsLine(start, end, v1, v2);
        if(result.intersects)
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
    const math::Vector& temp = point - quad.bottom_left;
    const math::Vector& size = math::Size(quad);

    // We need to flip the y axis
    return math::Vector(temp.x / size.x, (size.y - temp.y) / size.y);
}

float math::NormalizeAngle(float radians)
{
    radians = std::fmod(radians, math::PI() * 2.0f);
    if(radians < 0.0f)
        radians += math::PI() * 2.0f;
    return radians;
}

math::LineIntersectionResult math::LineIntersectsLine(
    const math::Vector& start_first, const math::Vector& end_first,
    const math::Vector& start_second, const math::Vector& end_second)
{
    const math::Vector& s1 = end_first - start_first;
    const math::Vector& s2 = end_second - start_second;

    const math::Vector& u = start_first - start_second;

    const float ip = 1.0f / (-s2.x * s1.y + s1.x * s2.y);

    const float s = (-s1.y * u.x + s1.x * u.y) * ip;
    const float t = ( s2.x * u.y - s2.y * u.x) * ip;

    LineIntersectionResult result;
    result.intersects = (s >= 0.0f && s <= 1.0f && t >= 0.0f && t <= 1.0f);
    result.intersection_point = start_first + (s1 * t);

    return result;
}

bool math::IsPrettyMuchEquals(float left, float right, float tolerance)
{
    return std::fabs(left - right) <= tolerance;
}
