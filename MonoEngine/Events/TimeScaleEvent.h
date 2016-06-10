//
//  TimeScaleEvent.h
//  MonoiOS
//
//  Created by Niklas Damberg on 10/06/16.
//
//

#pragma once

namespace event
{
    struct TimeScaleEvent
    {
        constexpr TimeScaleEvent(float scale)
            : time_scale(scale)
        { }

        const float time_scale;
    };
}