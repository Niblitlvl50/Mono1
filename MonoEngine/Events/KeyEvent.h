
#pragma once

#include "System/Keycodes.h"

namespace event
{
    struct KeyDownEvent
    {
        constexpr KeyDownEvent(Key::Keycode key, bool ctrl, bool shift, bool alt)
            : key(key),
              ctrl(ctrl),
              shift(shift),
              alt(alt)
        { }

        const Key::Keycode key;
        const bool ctrl;
        const bool shift;
        const bool alt;
    };

    struct KeyUpEvent
    {
        KeyUpEvent(Key::Keycode key, bool ctrl, bool shift, bool alt)
            : key(key),
              ctrl(ctrl),
              shift(shift),
              alt(alt)
        { }

        const Key::Keycode key;
        const bool ctrl;
        const bool shift;
        const bool alt;
    };
}

