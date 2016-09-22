//
//  MathFunctions.cpp
//  Mono1
//
//  Created by Niblit on 2012-07-27.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "MathFunctions.h"
#include "Vector2f.h"
#include "Quad.h"

#include <cmath>

bool math::PointInsideQuad(const math::Vector2f& point, const math::Quad& quad)
{
    if(point.x > quad.mA.x &&
       point.x < quad.mB.x &&
       point.y > quad.mA.y &&
       point.y < quad.mB.y)
        return true;
    
    return false;
}

bool math::QuadOverlaps(const math::Quad& left, const math::Quad& right)
{
    const Vector2f& left1 = left.mA;
    const Vector2f& left2 = left.mB;
    
    const Vector2f& right1 = right.mA;
    const Vector2f& right2 = right.mB;
    
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

float math::AngleBetweenPoints(const math::Vector2f& first, const math::Vector2f& second)
{
    return std::atan2(second.y - first.y, second.x - first.x);
}

math::Vector2f math::CentroidOfPolygon(const std::vector<math::Vector2f>& points)
{
    math::Vector2f centroid;
    float area = 0.0f;

    const auto func = [&centroid, &area](const math::Vector2f& first, const math::Vector2f& second) {
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

bool math::IsPolygonClockwise(const std::vector<math::Vector2f>& points)
{
    float sum = 0.0f;

    for(size_t point_index = 0; point_index < points.size(); ++point_index)
    {
        const math::Vector2f& v1 = points[point_index];
        const math::Vector2f& v2 = points[(point_index + 1) % points.size()];

        sum += (v2.x - v1.x) * (v2.y + v1.y);
    }

    return sum > 0.0f;
}

math::Vector2f math::MapVectorInQuad(const math::Vector2f& point, const math::Quad& quad)
{
    const math::Vector2f& temp = point - quad.mA;
    const math::Vector2f& size = quad.mB - quad.mA;

    // We need to flip the y axis
    return math::Vector2f(temp.x / size.x, (size.y - temp.y) / size.y);
}


