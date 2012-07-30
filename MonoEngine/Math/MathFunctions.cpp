//
//  MathFunctions.cpp
//  Mono1
//
//  Created by Niblit on 2012-07-27.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "MathFunctions.h"
#include "Quad.h"

bool Math::QuadOverlaps(const Math::Quad& left, const Math::Quad& right)
{
    const Vector2f& left1 = left.mA;
    const Vector2f  left2 = left.mA + left.mB;
    
    const Vector2f& right1 = right.mA;
    const Vector2f  right2 = right.mA + right.mB;
    
    if( left1.mX < right2.mX && left2.mX > right1.mX &&
        left1.mY < right2.mY && left2.mY > right1.mY )
        return true;
    
    return false;
}
