
#pragma once

#include "EventFwd.h"
#include "EventHandler/EventToken.h"
#include <functional>

namespace event
{
    using PauseEventFunc = std::function<mono::EventResult (const PauseEvent&)>;
    using QuitEventFunc = std::function<mono::EventResult (const QuitEvent&)>;
    using ApplicationEventFunc = std::function<mono::EventResult (const ApplicationEvent&)>;
    using SurfaceChangedEventFunc = std::function<mono::EventResult (const SurfaceChangedEvent&)>;
    using ActivatedEventFunc = std::function<mono::EventResult (const ActivatedEvent&)>;
    using KeyDownEventFunc = std::function<mono::EventResult (const KeyDownEvent&)>;
    using KeyUpEventFunc = std::function<mono::EventResult (const KeyUpEvent&)>;
    using TextInputEventFunc = std::function<mono::EventResult (const TextInputEvent&)>;
    using MouseDownEventFunc = std::function<mono::EventResult (const MouseDownEvent&)>;
    using MouseUpEventFunc = std::function<mono::EventResult (const MouseUpEvent&)>;
    using MouseMotionEventFunc = std::function<mono::EventResult (const MouseMotionEvent&)>;
    using MouseWheelEventFunc = std::function<mono::EventResult (const MouseWheelEvent&)>;
    using MultiGestureEventFunc = std::function<mono::EventResult (const MultiGestureEvent&)>;
    using TimeScaleEventFunc = std::function<mono::EventResult (const TimeScaleEvent&)>;
    using ControllerAddedFunc = std::function<mono::EventResult (const ControllerAddedEvent&)>;
    using ControllerRemovedFunc = std::function<mono::EventResult (const ControllerRemovedEvent&)>;
    using ControllerButtonDownFunc = std::function<mono::EventResult (const ControllerButtonDownEvent&)>;
}