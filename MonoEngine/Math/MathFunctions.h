//
//  MathFunctions.h
//  Mono1
//
//  Created by Niblit on 2012-07-27.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#pragma once

#include "Math.h"

namespace math
{    
    bool PointInsideQuad(const math::Vector2f& point, const math::Quad& quad);
    bool QuadOverlaps(const math::Quad& left, const math::Quad& right);
    void ResizeQuad(math::Quad& quad, float value, float aspect = 1.0f);
    
    float RadToDeg(float radians);
    float DegToRad(float degrees);
}
