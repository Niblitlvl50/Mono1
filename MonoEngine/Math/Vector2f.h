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
    
    inline Vector2f operator + (const Vector2f& left, const Vector2f& right)
    {
        return Vector2f(left.mX + right.mX, left.mY + right.mY);
    }

    inline Vector2f operator - (const Vector2f& left, const Vector2f& right)
    {
        return Vector2f(left.mX - right.mX, left.mY - right.mY);
    }

    inline Vector2f operator * (const Vector2f& left, float value)
    {
        return Vector2f(left.mX * value, left.mY * value);
    }
    
    inline Vector2f operator / (const Vector2f& left, const Vector2f& right)
    {
        return Vector2f(left.mX / right.mX, left.mY / right.mY);
    }
    
    inline Vector2f operator / (const Vector2f& left, float value)
    {
        return Vector2f(left.mX / value, left.mY / value);
    }
        
    inline void operator += (Vector2f& left, const Vector2f& right)
    {
        left.mX += right.mX;
        left.mY += right.mY;
    }
    
}

