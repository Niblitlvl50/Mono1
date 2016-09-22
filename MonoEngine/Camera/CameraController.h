//
//  CameraController.h
//  Mono1
//
//  Created by Niblit on 2012-08-26.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//


#pragma once

#include "MonoFwd.h"
#include "Events/EventFwd.h"
#include "EventHandler/EventToken.h"

namespace mono
{
    class CameraController
    {
    public:
        
        CameraController(ICamera* cam, EventHandler& eventHandler);
        ~CameraController();
        
    private:
        
        bool OnMultiGesture(const event::MultiGestureEvent& event);
        
        ICamera* mCam;
        EventHandler& mEventHandler;
        bool mEnabled;
        
        mono::EventToken<event::MultiGestureEvent> mMultiGestureToken;
    };
}
