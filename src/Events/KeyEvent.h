
#pragma once

#include "System/Keycodes.h"

namespace event
{
    struct KeyDownEvent
    {
        constexpr KeyDownEvent(Keycode key, bool ctrl, bool shift, bool alt, bool super)
            : key(key)
            , ctrl(ctrl)
            , shift(shift)
            , alt(alt)
            , super(super)
        { }

        const Keycode key;
        const bool ctrl;
        const bool shift;
        const bool alt;
        const bool super;
    };

    struct KeyUpEvent
    {
        KeyUpEvent(Keycode key, bool ctrl, bool shift, bool alt, bool super)
            : key(key)
            , ctrl(ctrl)
            , shift(shift)
            , alt(alt)
            , super(super)
        { }

        const Keycode key;
        const bool ctrl;
        const bool shift;
        const bool alt;
        const bool super;
    };
}

