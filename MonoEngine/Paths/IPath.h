//
//  IPath.h
//  Worksample_2
//
//  Created by Niklas Damberg on 13/11/13.
//
//

#pragma once

#include "MathFwd.h"

namespace mono
{
    struct IPath
    {
        virtual ~IPath()
        { }
        
        virtual float Length() const = 0;
        virtual math::Vector2f GetPositionByLength(float length) const = 0;

        virtual size_t Points() const = 0;
        virtual math::Vector2f GetPointByIndex(size_t index) const = 0;
    };
}
