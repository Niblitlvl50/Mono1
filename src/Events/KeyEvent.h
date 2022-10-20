
#pragma once

#include "System/Keycodes.h"
#include <cstdint>

namespace event
{
    struct KeyDownEvent
    {
        constexpr KeyDownEvent(Keycode key, bool ctrl, bool shift, bool alt, bool super, uint32_t timestamp)
            : key(key)
            , ctrl(ctrl)
            , shift(shift)
            , alt(alt)
            , super(super)
            , timestamp(timestamp)
        { }

        const Keycode key;
        const bool ctrl;
        const bool shift;
        const bool alt;
        const bool super;
        const uint32_t timestamp;
    };

    struct KeyUpEvent
    {
        KeyUpEvent(Keycode key, bool ctrl, bool shift, bool alt, bool super, uint32_t timestamp)
            : key(key)
            , ctrl(ctrl)
            , shift(shift)
            , alt(alt)
            , super(super)
            , timestamp(timestamp)
        { }

        const Keycode key;
        const bool ctrl;
        const bool shift;
        const bool alt;
        const bool super;
        const uint32_t timestamp;
    };
}

