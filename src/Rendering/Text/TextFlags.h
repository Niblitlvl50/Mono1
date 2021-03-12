
#pragma once

#include <cstdint>

namespace mono
{
    enum FontCentering : uint32_t
    {
        DEFAULT_CENTER = 0,
        VERTICAL = 1,
        HORIZONTAL = 2,

        HORIZONTAL_VERTICAL = HORIZONTAL | VERTICAL,
    };

    inline const char* FontCenterToString(uint32_t category)
    {
        switch(category)
        {
        case FontCentering::DEFAULT_CENTER:
            return "None";
        case FontCentering::VERTICAL:
            return "Vertical";
        case FontCentering::HORIZONTAL:
            return "Horizontal";
        };

        return "Unknown";
    };
}
