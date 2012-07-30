//
//  Quad.h
//  Mono1
//
//  Created by Niblit on 2012-05-02.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#pragma once

#include "Vector2f.h"

namespace Math
{
    struct Quad
    {
        Quad()
        { }
                
        Quad(float x, float y, float w, float h)
            : mA(x, y),
              mB(w, h)
        { }
                
        Math::Vector2f mA;
        Math::Vector2f mB;
    };
}
