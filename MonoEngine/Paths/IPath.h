//
//  IPath.h
//  King_worksample_OSX
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
    };
}
