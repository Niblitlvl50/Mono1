//
//  ShuttleController.h
//  Mono1
//
//  Created by Niblit on 2012-10-07.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#pragma once

#include "MonoFwd.h"
#include "EventFwd.h"
#include "EventToken.h"
#include "Vector2f.h"

namespace game
{
    class Shuttle;
    
    class ShuttleController
    {
    public:
        
        ShuttleController(game::Shuttle* shuttle, mono::EventHandler& eventHandler);
        ~ShuttleController();
        
        void OnMouseDown(const Event::MouseDownEvent& even);
        void OnMouseUp(const Event::MouseUpEvent& event);
        void OnMouseMotion(const Event::MouseMotionEvent& event);
        void OnMultiGesture(const Event::MultiGestureEvent& event);
        void OnKeyDown(const Event::KeyDownEvent& event);
        
    private:
        
        Shuttle* mShuttle;
        mono::EventHandler& mEventHandler;
        
        mono::EventToken<Event::MouseDownEvent> mMouseDownToken;
        mono::EventToken<Event::MouseUpEvent> mMouseUpToken;
        mono::EventToken<Event::MouseMotionEvent> mMouseMotionToken;
        mono::EventToken<Event::MultiGestureEvent> mMultiGestureToken;
        mono::EventToken<Event::KeyDownEvent> mKeyDownToken;

        bool mMouseDown;
        math::Vector2f mMouseDownPosition;
    };
}
