
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
