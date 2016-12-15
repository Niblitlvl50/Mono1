//
//  Vector.h
//  Mono1
//
//  Created by Niblit on 2012-04-09.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#pragma once

namespace math
{
    struct Vector
    {
        constexpr Vector()
        { }

        constexpr Vector(float x, float y)
            : x(x),
              y(y)
        { }

        float x = 0.0f;
        float y = 0.0f;
    };


    // Operators!
    Vector operator + (const Vector& left, const Vector& right);
    Vector operator - (const Vector& left, const Vector& right);
    Vector operator * (const Vector& left, float value);
    Vector operator * (const Vector& left, const Vector& right);
    Vector operator / (const Vector& left, const Vector& right);
    Vector operator / (const Vector& left, float value);

    Vector operator - (const Vector& vector);

    // Assigment operators!
    void operator *= (Vector& left, float value);
    void operator *= (Vector& left, const Vector& right);
    void operator += (Vector& left, const Vector& right);
    void operator -= (Vector& left, const Vector& right);

    bool operator == (const Vector& left, const Vector& right);

    // Calculates the length of a vector
    float Length(const Vector& vector);

    // Normalizes the vector
    void Normalize(Vector& vector);

    // Just a convineince vector declared to zero.
    constexpr Vector zeroVec = Vector();
}

