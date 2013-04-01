//
//  AnimatedDudeController.h
//  Mono1
//
//  Created by Niblit on 2012-04-11.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//


#pragma once

#include "EventFwd.h"
#include "EventToken.h"

namespace game
{
    class AnimatedDude;
    
    class AnimatedDudeController
    {
        
    public:
        
        AnimatedDudeController(AnimatedDude* dude);
        ~AnimatedDudeController();
        
        void OnKeyDown(const Event::KeyDownEvent& event);
        void OnMouseUp(const Event::MouseUpEvent& event);
        void OnMouseMotion(const Event::MouseMotionEvent& event);
        
    private:
        
        AnimatedDude* mDude;
        mono::EventToken<Event::KeyDownEvent> mKeyDownToken;
        mono::EventToken<Event::MouseUpEvent> mMouseUpToken;
        mono::EventToken<Event::MouseMotionEvent> mMouseMotionToken;
    };
}
