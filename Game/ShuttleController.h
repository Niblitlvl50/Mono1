//
//  ShuttleController.h
//  Mono1
//
//  Created by Niblit on 2012-10-07.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#pragma once

#include "EventFwd.h"
#include "EventToken.h"

namespace game
{
    class Shuttle;
    
    class ShuttleController
    {
    public:
        
        ShuttleController(game::Shuttle* shuttle);
        ~ShuttleController();
        
        void OnMouseDown(const Event::MouseDownEvent& even);
        void OnMouseUp(const Event::MouseUpEvent& event);
        
    private:
        
        void HandleOnShuttlePress();
        
        Shuttle* mShuttle;
        mono::EventToken<Event::MouseDownEvent> mMouseDownToken;
        mono::EventToken<Event::MouseUpEvent> mMouseUpToken;
    };
}
