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
    if(point.mX > quad.mA.mX &&
       point.mX < quad.mB.mX &&
       point.mY > quad.mA.mY &&
       point.mY < quad.mB.mY)
        return true;
    
    return false;
}

bool math::QuadOverlaps(const math::Quad& left, const math::Quad& right)
{
    const Vector2f& left1 = left.mA;
    const Vector2f& left2 = left.mB;
    
    const Vector2f& right1 = right.mA;
    const Vector2f& right2 = right.mB;
    
    if( left1.mX < right2.mX && left2.mX > right1.mX &&
        left1.mY < right2.mY && left2.mY > right1.mY )
        return true;
    
    return false;
}

void math::ResizeQuad(math::Quad& quad, float value, float aspect)
{
    const float resizeX = value * aspect;
    const float resizeY = value;
    
    quad.mA.mX -= (resizeX * 0.5f);
    quad.mA.mY -= (resizeY * 0.5f);

    quad.mB.mX += resizeX;
    quad.mB.mY += resizeY;
}

float math::AngleBetweenPoints(const math::Vector2f& first, const math::Vector2f& second)
{
    const float angle = std::atan2(second.mY - first.mY, second.mX - first.mX);
    return RadToDeg(angle);
}

float math::RadToDeg(float radians)
{
    return radians * 180.0f / math::PI;
}

float math::DegToRad(float degrees)
{
    return degrees * math::PI / 180.0f;
}


