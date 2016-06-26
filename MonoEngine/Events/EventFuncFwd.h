//
//  EventFuncFwd.h
//  Worksample_2
//
//  Created by Niklas Damberg on 09/11/13.
//
//

#pragma once

#include "EventFwd.h"
#include <functional>

namespace event
{
    typedef std::function<bool (const PauseEvent&)> PauseEventFunc;
    typedef std::function<bool (const QuitEvent&)> QuitEventFunc;
    typedef std::function<bool (const ApplicationEvent&)> ApplicationEventFunc;
    typedef std::function<bool (const SurfaceChangedEvent&)> SurfaceChangedEventFunc;
    typedef std::function<bool (const ActivatedEvent&)> ActivatedEventFunc;
    typedef std::function<bool (const KeyDownEvent&)> KeyDownEventFunc;
    typedef std::function<bool (const KeyUpEvent&)> KeyUpEventFunc;
    typedef std::function<bool (const TextInputEvent&)> TextInputEventFunc;
    typedef std::function<bool (const MouseDownEvent&)> MouseDownEventFunc;
    typedef std::function<bool (const MouseUpEvent&)> MouseUpEventFunc;
    typedef std::function<bool (const MouseMotionEvent&)> MouseMotionEventFunc;
    typedef std::function<bool (const MouseWheelEvent&)> MouseWheelEventFunc;
    typedef std::function<bool (const MultiGestureEvent&)> MultiGestureEventFunc;
    typedef std::function<bool (const TimeScaleEvent&)> TimeScaleEventFunc;
}