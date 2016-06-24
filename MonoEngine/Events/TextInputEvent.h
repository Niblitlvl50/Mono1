//
//  TextInputEvent.h
//  MonoiOS
//
//  Created by Niklas Damberg on 23/06/16.
//
//

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
