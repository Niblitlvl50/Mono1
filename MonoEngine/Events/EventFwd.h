//
//  EventFwd.h
//  Monolith1
//
//  Created by Niblit on 2012-01-07.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#pragma once

#include <functional>

namespace Event
{
    struct PauseEvent;
    typedef std::function<void (const PauseEvent&)> PauseEventFunc;
    
    struct QuitEvent;
    typedef std::function<void (const QuitEvent&)> QuitEventFunc;
    
    struct SurfaceChangedEvent;
    typedef std::function<void (const SurfaceChangedEvent&)> SurfaceChangedEventFunc;
    
    struct ActivatedEvent;
    typedef std::function<void (const ActivatedEvent&)> ActivatedEventFunc;
    
    struct KeyDownEvent;
    typedef std::function<void (const KeyDownEvent&)> KeyDownEventFunc;

    struct KeyUpEvent;
    typedef std::function<void (const KeyUpEvent&)> KeyUpEventFunc;
    
    struct MouseDownEvent;
    typedef std::function<void (const MouseDownEvent&)> MouseDownEventFunc;
    
    struct MouseUpEvent;
    typedef std::function<void (const MouseUpEvent&)> MouseUpEventFunc;
    
    struct MouseMotionEvent;
    typedef std::function<void (const MouseMotionEvent&)> MouseMotionEventFunc;
    
    struct MouseWheelEvent;
    typedef std::function<void (const MouseWheelEvent&)> MouseWheelEventFunc;
    
    struct MultiGestureEvent;
    typedef std::function<void (const MultiGestureEvent&)> MultiGestureEventFunc;
}

