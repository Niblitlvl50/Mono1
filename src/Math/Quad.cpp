
#include "Quad.h"
#include "MathFwd.h"
#include <algorithm>
#include <cmath>

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

float math::Width(const math::Quad& quad)
{
    return std::fabs(quad.mB.x - quad.mA.x);
}

float math::Height(const math::Quad& quad)
{
    return std::fabs(quad.mB.y - quad.mA.y);
}

float math::Left(const math::Quad& quad)
{
    return quad.mA.x;
}

float math::Right(const math::Quad& quad)
{
    return quad.mB.x;
}

float math::Top(const math::Quad& quad)
{
    return quad.mB.y;
}

float math::Bottom(const math::Quad& quad)
{
    return quad.mA.y;
}

math::Vector math::BottomLeft(const math::Quad& quad)
{
    return quad.mA;
}

math::Vector math::BottomRight(const math::Quad& quad)
{
    return math::Vector(quad.mB.x, quad.mA.y);
}

math::Vector math::BottomCenter(const math::Quad& quad)
{
    const float half_width = math::Width(quad) / 2.0f;
    return math::Vector(quad.mA.x + half_width, quad.mA.y);
}

math::Vector math::TopLeft(const math::Quad& quad)
{
    return math::Vector(quad.mA.x, quad.mB.y);
}

math::Vector math::TopCenter(const math::Quad& quad)
{
    const float half_width = (quad.mB.x - quad.mA.x) / 2.0f;
    return math::Vector(quad.mA.x + half_width, quad.mB.y);
}

math::Vector math::TopRight(const math::Quad& quad)
{
    return quad.mB;
}

math::Vector math::Center(const math::Quad& quad)
{
    const float half_width = math::Width(quad) / 2.0f;
    const float half_height = math::Height(quad) / 2.0f;
    return math::Vector(quad.mA.x + half_width, quad.mA.y + half_height);
}

const math::Quad math::InfQuad = Quad(-math::INF, -math::INF, math::INF, math::INF);
