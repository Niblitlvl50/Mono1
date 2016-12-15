//
//  ShuttleController.h
//  Mono1
//
//  Created by Niblit on 2012-10-07.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#pragma once

#include "MonoFwd.h"
#include "Events/EventFwd.h"
#include "EventHandler/EventToken.h"
#include "Math/Vector.h"

namespace game
{
    class Shuttle;
    
    class ShuttleController
    {
    public:
        
        ShuttleController(game::Shuttle* shuttle, mono::EventHandler& eventHandler);
        ~ShuttleController();
        
        bool OnMouseDown(const event::MouseDownEvent& even);
        bool OnMouseUp(const event::MouseUpEvent& event);
        bool OnMouseMotion(const event::MouseMotionEvent& event);
        bool OnKeyDown(const event::KeyDownEvent& event);
        bool OnKeyUp(const event::KeyUpEvent& event);
        
    private:
        
        Shuttle* mShuttle;
        mono::EventHandler& mEventHandler;
        
        mono::EventToken<event::MouseDownEvent> mMouseDownToken;
        mono::EventToken<event::MouseUpEvent> mMouseUpToken;
        mono::EventToken<event::MouseMotionEvent> mMouseMotionToken;
        mono::EventToken<event::KeyDownEvent> mKeyDownToken;
        mono::EventToken<event::KeyUpEvent> m_keyUpToken;

        bool mMouseDown;
        math::Vector mMouseDownPosition;
    };
}
