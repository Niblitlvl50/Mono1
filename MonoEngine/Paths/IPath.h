//
//  IPath.h
//  Worksample_2
//
//  Created by Niklas Damberg on 13/11/13.
//
//

#pragma once

#include "Math/MathFwd.h"
#include <vector>

namespace mono
{
    class IPath
    {
    public:
    
        virtual ~IPath()
        { }

        // Get total length of the path
        virtual float Length() const = 0;

        virtual const math::Vector& GetGlobalPosition() const = 0;
        virtual void SetGlobalPosition(const math::Vector& position) = 0;

        // Get a position from a length value
        virtual math::Vector GetPositionByLength(float length) const = 0;

        //virtual float GetLengthFromPosition(const math::Vector& position) const = 0;

        // Get a collection of all the points that defines the path
        virtual const std::vector<math::Vector>& GetPathPoints() const = 0;
    };
}
