//
//  Vector2f.h
//  Mono1
//
//  Created by Niblit on 2012-04-09.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#pragma once

namespace math
{
    struct Vector2f
    {
        Vector2f()
        { }

        Vector2f(float x, float y)
            : mX(x),
              mY(y)
        { }

        float mX = 0.0f;
        float mY = 0.0f;
    };


    // Operators!
    Vector2f operator + (const Vector2f& left, const Vector2f& right);
    Vector2f operator - (const Vector2f& left, const Vector2f& right);
    Vector2f operator * (const Vector2f& left, float value);
    Vector2f operator * (const Vector2f& left, const Vector2f& right);
    Vector2f operator / (const Vector2f& left, const Vector2f& right);
    Vector2f operator / (const Vector2f& left, float value);

    Vector2f operator - (const Vector2f& vector);

    // Assigment operators!
    void operator *= (Vector2f& left, float value);
    void operator += (Vector2f& left, const Vector2f& right);
    void operator -= (Vector2f& left, const Vector2f& right);

    bool operator == (const Vector2f& left, const Vector2f& right);

    // Calculates the length of a vector
    float Length(const Vector2f& vector);

    // Normalizes the vector
    void Normalize(Vector2f& vector);

    // Just a convineince vector declared to zero.
    extern const Vector2f zeroVec;
}

