/*
 *  MonolithEngine.h
 *  Monolith1
 *
 *  Created by Niblit on 2011-02-07.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once

#include "MonoPtrFwd.h"
#include "EventFwd.h"
#include "EventToken.h"

namespace mono
{
    class Engine
    {
    public:
        Engine(IWindowPtr view, ICameraPtr camera, EventHandler& eventHandler);
        ~Engine();
		
        void Run(IZonePtr zone);
		
    private:
		
        void OnPause(const Event::PauseEvent& event);
        void OnQuit(const Event::QuitEvent& event);
        void OnSurfaceChanged(const Event::SurfaceChangedEvent& event);
        void OnActivated(const Event::ActivatedEvent& event);
        void OnKeyUp(const Event::KeyUpEvent& event);        

        bool mPause;
        bool mQuit;

        IWindowPtr mWindow;
        ICameraPtr mCamera;
        EventHandler& mEventHandler;
        IInputHandlerPtr mInputHandler;

        EventToken<Event::PauseEvent> mPauseToken;
        EventToken<Event::QuitEvent> mQuitToken;
        EventToken<Event::SurfaceChangedEvent> mSurfaceChangedToken;
        EventToken<Event::ActivatedEvent> mActivatedToken;
        EventToken<Event::KeyUpEvent> mKeyUpToken;
    };
}

