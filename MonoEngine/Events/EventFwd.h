//
//  EventFwd.h
//  Monolith1
//
//  Created by Niblit on 2012-01-07.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#pragma once

#include <tr1/functional>

namespace Event
{
    struct PauseEvent;
    typedef std::tr1::function<void (const PauseEvent&)> PauseEventFunc;
    
    struct QuitEvent;
    typedef std::tr1::function<void (const QuitEvent&)> QuitEventFunc;
    
    struct SurfaceChangedEvent;
    typedef std::tr1::function<void (const SurfaceChangedEvent&)> SurfaceChangedEventFunc;
    
    struct ActivatedEvent;
    typedef std::tr1::function<void (const ActivatedEvent&)> ActivatedEventFunc;
    
    struct KeyDownEvent;
    typedef std::tr1::function<void (const KeyDownEvent&)> KeyDownEventFunc;

    struct KeyUpEvent;
    typedef std::tr1::function<void (const KeyUpEvent&)> KeyUpEventFunc;
    
    struct MouseDownEvent;
    typedef std::tr1::function<void (const MouseDownEvent&)> MouseDownEventFunc;
    
    struct MouseUpEvent;
    typedef std::tr1::function<void (const MouseUpEvent&)> MouseUpEventFunc;
    
    struct MouseMotionEvent;
    typedef std::tr1::function<void (const MouseMotionEvent&)> MouseMotionEventFunc;
    
    struct MouseWheelEvent;
    typedef std::tr1::function<void (const MouseWheelEvent&)> MouseWheelEventFunc;
    
    struct MultiGestureEvent;
    typedef std::tr1::function<void (const MultiGestureEvent&)> MultiGestureEventFunc;
}

