
#include "Quad.h"
#include "MathFwd.h"
#include <algorithm>
#include <cmath>

math::Quad math::operator * (const math::Quad& left, float value)
{
    return math::Quad(left.bottom_left * value, left.top_right * value);
}

math::Quad math::operator * (const math::Quad& left, const math::Vector& right)
{
    return math::Quad(left.bottom_left * right, left.top_right * right);
}

math::Quad math::operator + (const math::Quad& quad, const math::Vector& offset)
{
    return math::Quad(quad.bottom_left + offset, quad.top_right + offset);
}

bool math::operator == (const math::Quad& left, const math::Quad& right)
{
    return left.bottom_left == right.bottom_left && left.top_right == right.top_right;
}

void math::ExpandBy(math::Quad& quad, const math::Quad& other)
{
    quad.bottom_left.x = std::min(quad.bottom_left.x, other.bottom_left.x);
    quad.bottom_left.y = std::min(quad.bottom_left.y, other.bottom_left.y);
    quad.top_right.x = std::max(quad.top_right.x, other.top_right.x);
    quad.top_right.y = std::max(quad.top_right.y, other.top_right.y);
}

void math::ExpandBy(math::Quad& quad, const math::Vector& point)
{
    quad.bottom_left.x = std::min(quad.bottom_left.x, point.x);
    quad.bottom_left.y = std::min(quad.bottom_left.y, point.y);

    quad.top_right.x = std::max(quad.top_right.x, point.x);
    quad.top_right.y = std::max(quad.top_right.y, point.y);
}

void math::NormalizeQuad(math::Quad& quad)
{
    if(quad.bottom_left.x > quad.top_right.x)
        std::swap(quad.bottom_left.x, quad.top_right.x);

    if(quad.bottom_left.y > quad.top_right.y)
        std::swap(quad.bottom_left.y, quad.top_right.y);
}

float math::Width(const math::Quad& quad)
{
    return std::fabs(quad.top_right.x - quad.bottom_left.x);
}

float math::Height(const math::Quad& quad)
{
    return std::fabs(quad.top_right.y - quad.bottom_left.y);
}

math::Vector math::Size(const math::Quad& quad)
{
    return math::Vector(math::Width(quad), math::Height(quad));
}

float math::Ratio(const math::Quad& quad)
{
    const math::Vector& size = math::Size(quad);
    return size.x / size.y;
}

float math::Left(const math::Quad& quad)
{
    return quad.bottom_left.x;
}

float math::Right(const math::Quad& quad)
{
    return quad.top_right.x;
}

float math::Top(const math::Quad& quad)
{
    return quad.top_right.y;
}

float math::Bottom(const math::Quad& quad)
{
    return quad.bottom_left.y;
}

math::Vector math::BottomLeft(const math::Quad& quad)
{
    return quad.bottom_left;
}

math::Vector math::BottomRight(const math::Quad& quad)
{
    return math::Vector(quad.top_right.x, quad.bottom_left.y);
}

math::Vector math::BottomCenter(const math::Quad& quad)
{
    const float half_width = math::Width(quad) / 2.0f;
    return math::Vector(quad.bottom_left.x + half_width, quad.bottom_left.y);
}

math::Vector math::TopLeft(const math::Quad& quad)
{
    return math::Vector(quad.bottom_left.x, quad.top_right.y);
}

math::Vector math::TopCenter(const math::Quad& quad)
{
    const float half_width = (quad.top_right.x - quad.bottom_left.x) / 2.0f;
    return math::Vector(quad.bottom_left.x + half_width, quad.top_right.y);
}

math::Vector math::TopRight(const math::Quad& quad)
{
    return quad.top_right;
}

math::Vector math::RightCenter(const math::Quad& quad)
{
    const float half_height = (quad.top_right.y - quad.bottom_left.y) / 2.0f;
    return math::Vector(quad.top_right.x, quad.bottom_left.y + half_height);
}

math::Vector math::LeftCenter(const math::Quad& quad)
{
    const float half_height = (quad.top_right.y - quad.bottom_left.y) / 2.0f;
    return math::Vector(quad.bottom_left.x, quad.bottom_left.y + half_height);
}

math::Vector math::Center(const math::Quad& quad)
{
    const float half_width = math::Width(quad) / 2.0f;
    const float half_height = math::Height(quad) / 2.0f;
    return math::Vector(quad.bottom_left.x + half_width, quad.bottom_left.y + half_height);
}

float math::ShortestSide(const math::Quad& quad)
{
    const float width = math::Width(quad);
    const float height = math::Height(quad);
    return width < height ? width : height;
}

const math::Quad math::InfQuad = math::Quad(-math::INF, -math::INF, math::INF, math::INF);
const math::Quad math::InverseInfQuad = math::Quad(math::INF, math::INF, -math::INF, -math::INF);
