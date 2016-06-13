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
        
        void OnMouseDown(const event::MouseDownEvent& even);
        void OnMouseUp(const event::MouseUpEvent& event);
        void OnMouseMotion(const event::MouseMotionEvent& event);
        void OnMultiGesture(const event::MultiGestureEvent& event);
        void OnKeyDown(const event::KeyDownEvent& event);
        
    private:
        
        Shuttle* mShuttle;
        mono::EventHandler& mEventHandler;
        
        mono::EventToken<event::MouseDownEvent> mMouseDownToken;
        mono::EventToken<event::MouseUpEvent> mMouseUpToken;
        mono::EventToken<event::MouseMotionEvent> mMouseMotionToken;
        mono::EventToken<event::MultiGestureEvent> mMultiGestureToken;
        mono::EventToken<event::KeyDownEvent> mKeyDownToken;

        bool mMouseDown;
        math::Vector2f mMouseDownPosition;
    };
}
