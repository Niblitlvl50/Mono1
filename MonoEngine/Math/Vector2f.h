//
//  Vector2f.h
//  Mono1
//
//  Created by Niblit on 2012-04-09.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#pragma once

namespace Math
{
    struct Vector2f
    {
        Vector2f()
            : mX(0.0f),
              mY(0.0f)
        { }
        
        Vector2f(float x, float y)
            : mX(x),
              mY(y)
        { }
        
        float mX;
        float mY;
    };    
}

