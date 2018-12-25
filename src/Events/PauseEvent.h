
#pragma once

namespace event
{
    struct PauseEvent
    {
        constexpr PauseEvent(bool pause)
            : pause(pause)
        { }

        const bool pause;
    };
}
