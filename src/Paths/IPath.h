
#pragma once

#include "Math/MathFwd.h"
#include <vector>

namespace mono
{
    class IPath
    {
    public:
    
        virtual ~IPath() = default;

        // Get total length of the path
        virtual float Length() const = 0;

        // Get a position from a length value
        virtual math::Vector GetPositionByLength(float length) const = 0;

        //virtual float GetLengthFromPosition(const math::Vector& position) const = 0;

        // Get a collection of all the points that defines the path
        virtual const std::vector<math::Vector>& GetPathPoints() const = 0;
    };
}
