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

bool Math::PointInsideQuad(const Math::Vector2f& point, const Math::Quad& quad)
{
    if(point.mX > quad.mA.mX &&
       point.mX < quad.mB.mX &&
       point.mY > quad.mA.mY &&
       point.mY < quad.mB.mY)
        return true;
    
    return false;
}

bool Math::QuadOverlaps(const Math::Quad& left, const Math::Quad& right)
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

void Math::ResizeQuad(Math::Quad& quad, float value, float aspect)
{
    const float resizeX = value * aspect;
    const float resizeY = value;
    
    quad.mA.mX -= (resizeX * 0.5f);
    quad.mA.mY -= (resizeY * 0.5f);

    quad.mB.mX += resizeX;
    quad.mB.mY += resizeY;
}

