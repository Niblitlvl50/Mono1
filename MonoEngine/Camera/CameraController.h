
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
