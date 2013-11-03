/*
 *  MonolithEngine.h
 *  Monolith1
 *
 *  Created by Niblit on 2011-02-07.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once

#include "MonoFwd.h"
#include "EventFwd.h"
#include "EventToken.h"

namespace mono
{

    class Engine
    {
    public:
        Engine(unsigned int hz, IWindowPtr view, ICameraPtr camera, IZonePtr zone);
        ~Engine();
		
        void Run();
		
    private:
		
        void OnPause(const Event::PauseEvent& event);
        void OnQuit(const Event::QuitEvent& event);
        void OnSurfaceChanged(const Event::SurfaceChangedEvent& event);
        void OnActivated(const Event::ActivatedEvent& event);
        
        bool mPause;
        bool mQuit;
        unsigned int mHz;
		
        IWindowPtr mWindow;
        ICameraPtr mCamera;
        IZonePtr mZone;
        IInputHandlerPtr mInputHandler;

        EventToken<Event::PauseEvent> mPauseToken;
        EventToken<Event::QuitEvent> mQuitToken;
        EventToken<Event::SurfaceChangedEvent> mSurfaceChangedToken;
        EventToken<Event::ActivatedEvent> mActivatedToken;
    };

}

