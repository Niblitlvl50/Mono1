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
    using PauseEventFunc = std::function<bool (const PauseEvent&)>;
    using QuitEventFunc = std::function<bool (const QuitEvent&)>;
    using ApplicationEventFunc = std::function<bool (const ApplicationEvent&)>;
    using SurfaceChangedEventFunc = std::function<bool (const SurfaceChangedEvent&)>;
    using ActivatedEventFunc = std::function<bool (const ActivatedEvent&)>;
    using KeyDownEventFunc = std::function<bool (const KeyDownEvent&)>;
    using KeyUpEventFunc = std::function<bool (const KeyUpEvent&)>;
    using TextInputEventFunc = std::function<bool (const TextInputEvent&)>;
    using MouseDownEventFunc = std::function<bool (const MouseDownEvent&)>;
    using MouseUpEventFunc = std::function<bool (const MouseUpEvent&)>;
    using MouseMotionEventFunc = std::function<bool (const MouseMotionEvent&)>;
    using MouseWheelEventFunc = std::function<bool (const MouseWheelEvent&)>;
    using MultiGestureEventFunc = std::function<bool (const MultiGestureEvent&)>;
    using TimeScaleEventFunc = std::function<bool (const TimeScaleEvent&)>;
}