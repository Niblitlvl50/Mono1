//
//  MathFwd.h
//  Mono1
//
//  Created by Niblit on 2013-04-01.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#pragma once

#include <vector>
#include <limits>

namespace math
{
    static const float PI = 3.14159265358979f;
    static const float INF = std::numeric_limits<float>::infinity();

    struct Vector2f;
    struct Quad;
}

typedef std::vector<math::Vector2f> Vector2fCollection;
