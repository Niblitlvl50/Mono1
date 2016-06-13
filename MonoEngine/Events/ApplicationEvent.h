//
//  ApplicationEvent.h
//  MonoiOS
//
//  Created by Niklas Damberg on 06/12/15.
//
//

#pragma once

namespace event
{
    enum class ApplicationState
    {
        TERMINATING,
        ENTER_BACKGROUND,
        ENTER_FOREGROUND
    };

    struct ApplicationEvent
    {
        constexpr ApplicationEvent(ApplicationState state)
            : state(state)
        { }

        const ApplicationState state;
    };
}
