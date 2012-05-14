//
//  Quad.h
//  Mono1
//
//  Created by Niblit on 2012-05-02.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#pragma once

namespace Math
{
    struct Quad
    {
        Quad()
            : mX(0.0f),
              mY(0.0f),
              mW(1.0f),
              mH(1.0f)
        { }
        
        Quad(float x, float y, float w, float h)
            : mX(x),
              mY(y),
              mW(w),
              mH(h)
        { }
        
        float mX;
        float mY;
        float mW;
        float mH;
    };
}
