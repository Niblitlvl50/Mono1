//
//  Quad.h
//  Mono1
//
//  Created by Niblit on 2012-05-02.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#pragma once

#include "Vector2f.h"

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
    
    Quad operator * (const Quad& left, float value);
    Quad operator * (const Quad& left, const Vector2f& right);
    
    //!
    //! Be aware that this only checks for max/min values.
    //! If you use the Quad as position and size vectors this
    //! will not do what you think. 
    //!
    void operator |= (Quad& left, const Quad& right);
    
    extern const Quad zeroQuad;
}
