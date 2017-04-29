
#pragma once

namespace game
{
    struct RemoveEntityEvent
    {
        constexpr RemoveEntityEvent(unsigned int id)
            : id(id)
        { }

        const unsigned int id;
    };
}
