//
//  Quad.h
//  Mono1
//
//  Created by Niblit on 2012-05-02.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#pragma once

#include "Vector.h"

namespace math
{
    struct Quad
    {
        constexpr Quad()
        { }

        constexpr Quad(float x, float y, float w, float h)
            : mA(x, y),
              mB(w, h)
        { }

        constexpr Quad(const Vector& a, const Vector& b)
            : mA(a),
              mB(b)
        { }

        Vector mA;
        Vector mB;
    };
    
    Quad operator * (const Quad& left, float value);
    Quad operator * (const Quad& left, const Vector& right);
    
    //!
    //! Be aware that this only checks for max/min values.
    //! If you use the Quad as position and size vectors this
    //! will not do what you think. 
    //!
    void operator |= (Quad& left, const Quad& right);

    void operator |= (Quad& left, const Vector& right);

    bool operator == (const Quad& left, const Quad& right);

    //! Zero quad defined for convenience
    constexpr Quad zeroQuad = Quad(zeroVec, zeroVec);
}
