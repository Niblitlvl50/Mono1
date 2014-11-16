//
//  Point.h
//  Worksample_2
//
//  Created by Niklas Damberg on 09/11/13.
//
//

#pragma once

namespace math
{
    struct Point
    {
        Point()
            : mX(0),
              mY(0)
        { }
        Point(int x, int y)
            : mX(x),
              mY(y)
        { }
        int mX;
        int mY;
    };
    
    Point operator + (const math::Point& left, const math::Point& right);
    Point operator - (const math::Point& left, const math::Point& right);
    Point operator / (const math::Point& left, int value);
}
