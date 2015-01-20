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
    const float angle = std::atan2(second.y - first.y, second.x - first.x);
    return ToDegrees(angle);
}



