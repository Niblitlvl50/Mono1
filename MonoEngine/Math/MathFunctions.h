//
//  MathFunctions.h
//  Mono1
//
//  Created by Niblit on 2012-07-27.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#pragma once

namespace Math
{
    struct Vector2f;
    struct Quad;
    
    bool PointInsideQuad(const Math::Vector2f& point, const Math::Quad& quad);
    bool QuadOverlaps(const Math::Quad& left, const Math::Quad& right);
    void ResizeQuad(Math::Quad& quad, float value, float aspect = 1.0f);
}
