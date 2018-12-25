
#include "Point.h"

math::Point math::operator + (const math::Point& left, const math::Point& right)
{
    return math::Point(left.x + right.x, left.y + right.y);
}

math::Point math::operator - (const math::Point& left, const math::Point& right)
{
    return math::Point(left.x - right.x, left.y - right.y);
}

math::Point math::operator / (const math::Point& left, int value)
{
    return math::Point(left.x / value, left.y / value);
}
