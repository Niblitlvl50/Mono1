
#pragma once

#include <cfloat>

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

    void Normalize(Vector& vector);
    math::Vector Normalize(const math::Vector& vector);
    float Length(const Vector& vector);
    float LengthSquared(const Vector& vector);
    float Dot(const Vector& first, const Vector& second);

    bool IsPrettyMuchEquals(const Vector& left, const Vector& right, float tolerance = FLT_EPSILON);

    // Just a convineince vector declared to zero.
    constexpr Vector ZeroVec = Vector();
}

