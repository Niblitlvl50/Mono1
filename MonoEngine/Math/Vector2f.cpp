//
//  Vector2f.cpp
//  MonoiOS
//
//  Created by Niklas Damberg on 03/11/13.
//
//

#include "Vector2f.h"
#include <cmath>

math::Vector2f math::operator + (const math::Vector2f& left, const math::Vector2f& right)
{
    return Vector2f(left.x + right.x, left.y + right.y);
}

math::Vector2f math::operator - (const math::Vector2f& left, const math::Vector2f& right)
{
    return Vector2f(left.x - right.x, left.y - right.y);
}

math::Vector2f math::operator * (const math::Vector2f& left, float value)
{
    return Vector2f(left.x * value, left.y * value);
}

math::Vector2f math::operator * (const math::Vector2f& left, const math::Vector2f& right)
{
    return math::Vector2f(left.x * right.x, left.y * right.y);
}

math::Vector2f math::operator / (const math::Vector2f& left, const math::Vector2f& right)
{
    return Vector2f(left.x / right.x, left.y / right.y);
}

math::Vector2f math::operator / (const math::Vector2f& left, float value)
{
    return Vector2f(left.x / value, left.y / value);
}

math::Vector2f math::operator - (const math::Vector2f& vector)
{
    return Vector2f(-vector.x, -vector.y);
}

void math::operator *= (math::Vector2f& left, float value)
{
    left.x *= value;
    left.y *= value;
}

void math::operator += (math::Vector2f& left, const math::Vector2f& right)
{
    left.x += right.x;
    left.y += right.y;
}

void math::operator -= (math::Vector2f& left, const math::Vector2f& right)
{
    left.x -= right.x;
    left.y -= right.y;
}

bool math::operator == (const math::Vector2f& left, const math::Vector2f& right)
{
    return left.x == right.x &&
           left.y == right.y;
}

float math::Length(const math::Vector2f& vector)
{
    return std::sqrt(vector.x * vector.x + vector.y * vector.y);
}

void math::Normalize(math::Vector2f& vector)
{
    const float length = Length(vector);
    vector.x /= length;
    vector.y /= length;
}
