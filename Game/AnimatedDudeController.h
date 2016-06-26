//
//  AnimatedDudeController.h
//  Mono1
//
//  Created by Niblit on 2012-04-11.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//


#pragma once

#include "MonoFwd.h"
#include "EventFwd.h"
#include "EventToken.h"

namespace game
{
    class AnimatedDude;
    
    class AnimatedDudeController
    {
    public:
        
        AnimatedDudeController(AnimatedDude* dude, mono::EventHandler& eventHandler);
        ~AnimatedDudeController();
        
        bool OnKeyDown(const event::KeyDownEvent& event);
        
    private:
        
        AnimatedDude* mDude;
        mono::EventHandler& mEventHandler;        
        mono::EventToken<event::KeyDownEvent> mKeyDownToken;
    };
}
