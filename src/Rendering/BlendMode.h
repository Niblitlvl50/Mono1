
#pragma once

namespace mono
{
    enum class BlendMode : int
    {
        SOURCE_ALPHA,
        ONE
    };

    constexpr const char* blend_mode_strings[] = {
        "Source Alpha",
        "One",
    };

    inline const char* BlendModeToString(BlendMode blend_mode)
    {
        return blend_mode_strings[static_cast<int>(blend_mode)];
    }
}
