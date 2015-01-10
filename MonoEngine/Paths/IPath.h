//
//  IPath.h
//  Worksample_2
//
//  Created by Niklas Damberg on 13/11/13.
//
//

#pragma once

#include "MathFwd.h"
#include <vector>

namespace mono
{
    struct IPath
    {
        virtual ~IPath()
        { }

        // Get total length of the path
        virtual float Length() const = 0;

        // Get a position from a length value
        virtual math::Vector2f GetPositionByLength(float length) const = 0;

        // Get a collection of all the points that defines the path
        virtual const std::vector<math::Vector2f>& GetPathPoints() const = 0;
    };
}
