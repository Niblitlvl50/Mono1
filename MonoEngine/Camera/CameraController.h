//
//  CameraController.h
//  Mono1
//
//  Created by Niblit on 2012-08-26.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//


#pragma once

#include "MonoFwd.h"
#include "EventFwd.h"
#include "EventToken.h"

namespace mono
{
    class CameraController
    {
    public:
        
        CameraController(ICamera* cam, EventHandler& eventHandler);
        ~CameraController();
        
    private:
        
        void OnMultiGesture(const Event::MultiGestureEvent& event);
        
        ICamera* mCam;
        EventHandler& mEventHandler;
        bool mEnabled;
        
        mono::EventToken<Event::MultiGestureEvent> mMultiGestureToken;
    };
}
