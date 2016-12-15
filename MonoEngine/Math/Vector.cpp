//
//  Vector.cpp
//  MonoiOS
//
//  Created by Niklas Damberg on 03/11/13.
//
//

#include "Vector.h"
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

float math::Length(const math::Vector& vector)
{
    return std::sqrt(vector.x * vector.x + vector.y * vector.y);
}

void math::Normalize(math::Vector& vector)
{
    const float length = Length(vector);
    if(length == 0.0f)
        return;

    vector.x /= length;
    vector.y /= length;
}
