
#pragma once

namespace event
{
    enum class ScreenMode
    {
        WINDOWED,
        FULLSCREEN,
        FULLSCREEN_DESKTOP
    };

    struct ScreenEvent
    {
        constexpr ScreenEvent(ScreenMode screen_mode)
            : screen_mode(screen_mode)
        { }
        const ScreenMode screen_mode;
    };
}
