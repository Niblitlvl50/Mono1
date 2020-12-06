
#include "Vector.h"
#include "MathFunctions.h"
#include <cmath>

math::Vector math::operator + (const math::Vector& left, const math::Vector& right)
{
    return Vector(left.x + right.x, left.y + right.y);
}

math::Vector math::operator - (const math::Vector& left, const math::Vector& right)
{
    return Vector(left.x - right.x, left.y - right.y);
}

math::Vector math::operator * (const math::Vector& left, float value)
{
    return Vector(left.x * value, left.y * value);
}

math::Vector math::operator * (const math::Vector& left, const math::Vector& right)
{
    return math::Vector(left.x * right.x, left.y * right.y);
}

math::Vector math::operator / (const math::Vector& left, const math::Vector& right)
{
    return Vector(left.x / right.x, left.y / right.y);
}

math::Vector math::operator / (const math::Vector& left, float value)
{
    return Vector(left.x / value, left.y / value);
}

math::Vector math::operator - (const math::Vector& vector)
{
    return Vector(-vector.x, -vector.y);
}

void math::operator *= (math::Vector& left, float value)
{
    left.x *= value;
    left.y *= value;
}

void math::operator *= (math::Vector& left, const math::Vector& right)
{
    left.x *= right.x;
    left.y *= right.y;
}

void math::operator += (math::Vector& left, const math::Vector& right)
{
    left.x += right.x;
    left.y += right.y;
}

void math::operator -= (math::Vector& left, const math::Vector& right)
{
    left.x -= right.x;
    left.y -= right.y;
}

bool math::operator == (const math::Vector& left, const math::Vector& right)
{
    return left.x == right.x &&
           left.y == right.y;
}

bool math::operator != (const math::Vector& left, const math::Vector& right)
{
    return !(left == right);
}

void math::Normalize(math::Vector& vector)
{
    const float length = Length(vector);
    if(length == 0.0f)
        return;

    vector.x /= length;
    vector.y /= length;
}

math::Vector math::Normalized(const math::Vector& vector)
{
    math::Vector temp_vector = vector;
    Normalize(temp_vector);
    return temp_vector;
}

float math::Length(const math::Vector& vector)
{
    return std::sqrt(vector.x * vector.x + vector.y * vector.y);
}

float math::LengthSquared(const math::Vector& vector)
{
    return vector.x * vector.x + vector.y * vector.y;
}

float math::Dot(const math::Vector& first, const math::Vector& second)
{
   return (first.x * second.x) + (first.y * second.y);
}

bool math::IsPrettyMuchEquals(const math::Vector& left, const math::Vector& right, float tolerance)
{
    return
        IsPrettyMuchEquals(left.x, right.x, tolerance) &&
        IsPrettyMuchEquals(left.y, right.y, tolerance);
}
