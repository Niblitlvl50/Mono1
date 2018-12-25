
#pragma once

namespace math
{
    struct Point
    {
        constexpr Point()
        { }

        constexpr Point(int x, int y)
            : x(x),
              y(y)
        { }

        int x = 0;
        int y = 0;
    };
    
    Point operator + (const math::Point& left, const math::Point& right);
    Point operator - (const math::Point& left, const math::Point& right);
    Point operator / (const math::Point& left, int value);
}
