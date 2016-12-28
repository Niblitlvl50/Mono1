
#pragma once

namespace game
{
    struct RemoveEntityEvent
    {
        constexpr RemoveEntityEvent(uint id)
            : id(id)
        { }

        const uint id;
    };
}
