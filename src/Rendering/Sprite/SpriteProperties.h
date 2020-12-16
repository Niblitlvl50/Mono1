
#pragma once

#include <cstdint>

namespace mono
{
    enum SpriteProperty : uint32_t
    {
        SP_NONE = 0,
        WIND_SWAY = 1,
        SHADOW = 2,
    };

    inline const char* SpritePropertyToString(uint32_t category)
    {
        switch(category)
        {
        case SpriteProperty::SP_NONE:
            return "None";
        case SpriteProperty::WIND_SWAY:
            return "Wind Sway";
        case SpriteProperty::SHADOW:
            return "Shadow";
        };

        return "Unknown";
    };
}
