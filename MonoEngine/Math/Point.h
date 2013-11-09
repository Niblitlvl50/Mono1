//
//  Point.h
//  King_worksample_OSX
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
}
