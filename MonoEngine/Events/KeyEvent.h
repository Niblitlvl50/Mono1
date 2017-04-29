
#pragma once

namespace event
{
    struct KeyDownEvent
    {
        constexpr KeyDownEvent(unsigned int key, bool ctrl, bool shift, bool alt)
            : key(key),
              ctrl(ctrl),
              shift(shift),
              alt(alt)
        { }
        
        const unsigned int key;
        const bool ctrl;
        const bool shift;
        const bool alt;
    };

    struct KeyUpEvent
    {
        constexpr KeyUpEvent(unsigned int key, bool ctrl, bool shift, bool alt)
            : key(key),
              ctrl(ctrl),
              shift(shift),
              alt(alt)
        { }
        
        const unsigned int key;
        const bool ctrl;
        const bool shift;
        const bool alt;
    };
}

