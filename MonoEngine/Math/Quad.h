//
//  Quad.h
//  Mono1
//
//  Created by Niblit on 2012-05-02.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#pragma once

#include "Vector2f.h"
#include <algorithm>

namespace math
{
    struct Quad
    {
        Quad()
        { }
        Quad(float x, float y, float w, float h)
            : mA(x, y),
              mB(w, h)
        { }
        Quad(const Vector2f& a, const Vector2f& b)
            : mA(a),
              mB(b)
        { }
        Vector2f mA;
        Vector2f mB;
    };
    
    inline Quad operator * (const Quad& left, float value)
    {
        return Quad(left.mA * value, left.mB * value);
    }
    
    //!
    //! Be aware that this only checks for max/min values.
    //! If you use the Quad as position and size vectors this
    //! will not do what you think. 
    //!
    inline void operator |= (Quad& left, const Quad& right)
    {
        left.mA.mX = std::min(left.mA.mX, right.mA.mX);
        left.mA.mY = std::min(left.mA.mY, right.mA.mY);
        left.mB.mX = std::max(left.mB.mX, right.mB.mX);
        left.mB.mY = std::max(left.mB.mY, right.mB.mY);        
    }

}
