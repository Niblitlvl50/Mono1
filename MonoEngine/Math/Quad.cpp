//
//  Quad.cpp
//  MonoiOS
//
//  Created by Niklas Damberg on 03/11/13.
//
//

#include "Quad.h"
#include <algorithm>


math::Quad math::operator * (const math::Quad& left, float value)
{
    return math::Quad(left.mA * value, left.mB * value);
}

math::Quad math::operator * (const math::Quad& left, const math::Vector2f& right)
{
    return math::Quad(left.mA * right, left.mB * right);
}

void math::operator |= (math::Quad& left, const math::Quad& right)
{
    left.mA.mX = std::min(left.mA.mX, right.mA.mX);
    left.mA.mY = std::min(left.mA.mY, right.mA.mY);
    left.mB.mX = std::max(left.mB.mX, right.mB.mX);
    left.mB.mY = std::max(left.mB.mY, right.mB.mY);
}

const math::Quad zeroQuad = math::Quad(math::zeroVec, math::zeroVec);