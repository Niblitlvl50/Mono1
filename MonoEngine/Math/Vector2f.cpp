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
    return Vector2f(left.mX + right.mX, left.mY + right.mY);
}

math::Vector2f math::operator - (const math::Vector2f& left, const math::Vector2f& right)
{
    return Vector2f(left.mX - right.mX, left.mY - right.mY);
}

math::Vector2f math::operator * (const math::Vector2f& left, float value)
{
    return Vector2f(left.mX * value, left.mY * value);
}

math::Vector2f math::operator * (const math::Vector2f& left, const math::Vector2f& right)
{
    return math::Vector2f(left.mX * right.mX, left.mY * right.mY);
}

math::Vector2f math::operator / (const math::Vector2f& left, const math::Vector2f& right)
{
    return Vector2f(left.mX / right.mX, left.mY / right.mY);
}

math::Vector2f math::operator / (const math::Vector2f& left, float value)
{
    return Vector2f(left.mX / value, left.mY / value);
}

void math::operator *= (math::Vector2f& left, float value)
{
    left.mX *= value;
    left.mY *= value;
}

void math::operator += (math::Vector2f& left, const math::Vector2f& right)
{
    left.mX += right.mX;
    left.mY += right.mY;
}

void math::operator -= (math::Vector2f& left, const math::Vector2f& right)
{
    left.mX -= right.mX;
    left.mY -= right.mY;
}

bool math::operator == (const math::Vector2f& left, const math::Vector2f& right)
{
    return left.mX == right.mX &&
           left.mY == right.mY;
}

float math::Length(const math::Vector2f& vector)
{
    return std::sqrt(vector.mX * vector.mX + vector.mY * vector.mY);
}

void math::Normalize(math::Vector2f& vector)
{
    const float length = Length(vector);
    vector.mX /= length;
    vector.mY /= length;
}

const math::Vector2f math::zeroVec = math::Vector2f(0.0f, 0.0f);
