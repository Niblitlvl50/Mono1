
#pragma once

namespace event
{
    struct SurfaceChangedEvent
    {
        constexpr SurfaceChangedEvent(int w, int h)
            : width(w),
              height(h)
        { }
        
        const int width;
        const int height;
    };
}

