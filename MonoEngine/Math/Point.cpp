//
//  Point.cpp
//  Worksample_2
//
//  Created by Niklas Damberg on 09/11/13.
//
//

#include "Point.h"

math::Point math::operator + (const math::Point& left, const math::Point& right)
{
    return math::Point(left.mX + right.mX, left.mY + right.mY);
}

math::Point math::operator - (const math::Point& left, const math::Point& right)
{
    return math::Point(left.mX - right.mX, left.mY - right.mY);
}

math::Point math::operator / (const math::Point& left, int value)
{
    return math::Point(left.mX / value, left.mY / value);
}
