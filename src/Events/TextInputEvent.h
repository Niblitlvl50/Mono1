
#pragma once

namespace event
{
    struct TextInputEvent
    {
        TextInputEvent(const char* text)
            : text(text)
        { }

        const char* text;
    };
}
