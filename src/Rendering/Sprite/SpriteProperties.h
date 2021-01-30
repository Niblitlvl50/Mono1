
#pragma once

#include <cstdint>

#define ENUM_BIT(n) (1 << (n))

namespace mono
{
    enum SpriteProperty : uint32_t
    {
        SP_NONE         = 0,
        WIND_SWAY       = ENUM_BIT(0),
        SHADOW          = ENUM_BIT(1),
        FLIP_HORIZONTAL = ENUM_BIT(2),
        FLIP_VERTICAL   = ENUM_BIT(3),
        FLASH           = ENUM_BIT(4),
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
        case SpriteProperty::FLIP_HORIZONTAL:
            return "Flip Horizontal";
        case SpriteProperty::FLIP_VERTICAL:
            return "Flip Vertical";
        case SpriteProperty::FLASH:
            return "Flash";
        };

        return "Unknown";
    };
}
