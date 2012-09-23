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
        
        CameraController(ICamera* cam);
        ~CameraController();
        
        void OnMouseWheel(const Event::MouseWheelEvent& event);
        void OnMultiGesture(const Event::MultiGestureEvent& event);
        
        mono::EventToken<Event::MouseWheelEvent> mMouseWheelToken;
        mono::EventToken<Event::MultiGestureEvent> mMultiGestureToken; 

        ICamera* mCam;
    };
}
