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

namespace Event
{
    typedef std::function<void (const PauseEvent&)> PauseEventFunc;
    typedef std::function<void (const QuitEvent&)> QuitEventFunc;
    typedef std::function<void (const SurfaceChangedEvent&)> SurfaceChangedEventFunc;
    typedef std::function<void (const ActivatedEvent&)> ActivatedEventFunc;
    typedef std::function<void (const KeyDownEvent&)> KeyDownEventFunc;
    typedef std::function<void (const KeyUpEvent&)> KeyUpEventFunc;
    typedef std::function<void (const MouseDownEvent&)> MouseDownEventFunc;
    typedef std::function<void (const MouseUpEvent&)> MouseUpEventFunc;
    typedef std::function<void (const MouseMotionEvent&)> MouseMotionEventFunc;
    typedef std::function<void (const MouseWheelEvent&)> MouseWheelEventFunc;
    typedef std::function<void (const MultiGestureEvent&)> MultiGestureEventFunc;
}
