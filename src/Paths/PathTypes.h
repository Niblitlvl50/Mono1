
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
}
