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

math::Quad math::operator * (const math::Quad& left, const math::Vector& right)
{
    return math::Quad(left.mA * right, left.mB * right);
}

void math::operator |= (math::Quad& left, const math::Quad& right)
{
    left.mA.x = std::min(left.mA.x, right.mA.x);
    left.mA.y = std::min(left.mA.y, right.mA.y);
    left.mB.x = std::max(left.mB.x, right.mB.x);
    left.mB.y = std::max(left.mB.y, right.mB.y);
}

void math::operator |= (math::Quad& left, const math::Vector& right)
{
    left.mA.x = std::min(left.mA.x, right.x);
    left.mA.y = std::min(left.mA.y, right.y);

    left.mB.x = std::max(left.mB.x, right.x);
    left.mB.y = std::max(left.mB.y, right.y);
}

bool math::operator == (const math::Quad& left, const math::Quad& right)
{
    return left.mA == right.mA && left.mB == right.mB;
}
