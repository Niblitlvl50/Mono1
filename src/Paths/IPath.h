
#pragma once

#include "Math/MathFwd.h"
#include "Math/Vector.h"
#include <vector>

namespace mono
{
    struct PositionResult
    {
        bool valid_position;
        math::Vector path_position;
    };

    struct LengthResult
    {
        bool valid_length;
        float path_length;
    };

    class IPath
    {
    public:
    
        virtual ~IPath() = default;

        // Get total length of the path
        virtual float Length() const = 0;

        // Get a position from a length value
        virtual PositionResult GetPositionByLength(float length) const = 0;

        virtual LengthResult GetLengthFromPosition(const math::Vector& position) const = 0;

        // Get the end point of the path
        virtual math::Vector GetEndPoint() const = 0;

        // Get a collection of all the points that defines the path
        virtual const std::vector<math::Vector>& GetPathPoints() const = 0;
    };
}
