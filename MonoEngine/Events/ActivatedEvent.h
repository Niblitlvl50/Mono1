
#pragma once

namespace event
{
    struct ActivatedEvent
    {
        constexpr ActivatedEvent(bool gain)
            : gain(gain)
        { }
        
        const bool gain;
    };
}
