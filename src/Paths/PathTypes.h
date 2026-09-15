
#pragma once

#include <cstdint>

namespace mono
{
    enum class PathType : uint32_t
    {
        REGULAR,
        BEZIER_QUADRATIC,
        BEZIER_CUBIC,
    };

    constexpr const char* path_type_strings[] = {
        "Regular",
        "Quadratic",
        "Cubic",
    };

    inline const char* PathTypeToString(PathType path_type)
    {
        return path_type_strings[(int)path_type];
    }

    // Returns true if the point at `index` is an anchor (on-curve) point for the given path type.
    // Cubic layout:    P0, C1out, C2in, P3, [C2in, P3]* — anchors at index 0 and odd indices >= 3.
    // Quadratic layout: A0, C0, A1, C1, ...              — anchors at even indices.
    inline bool IsAnchorPoint(PathType type, int index)
    {
        switch(type)
        {
        case PathType::BEZIER_CUBIC:
            return index == 0 || (index >= 3 && index % 2 == 1);
        case PathType::BEZIER_QUADRATIC:
            return index % 2 == 0;
        default:
            return true;
        }
    }
}
