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
        Engine(const IWindowPtr& view, const ICameraPtr& camera, EventHandler& eventHandler);
        ~Engine();
		
        void Run(IZonePtr zone);
		
    private:
		
        void OnPause(const Event::PauseEvent& event);
        void OnQuit(const Event::QuitEvent& event);
        void OnApplication(const Event::ApplicationEvent& event);
        void OnSurfaceChanged(const Event::SurfaceChangedEvent& event);
        void OnActivated(const Event::ActivatedEvent& event);
        void OnTimeScale(const event::TimeScaleEvent& event);

        bool mPause;
        bool mQuit;
        bool mUpdateLastTime;

        float mTimeScale;

        IWindowPtr mWindow;
        ICameraPtr mCamera;
        EventHandler& mEventHandler;
        IInputHandlerPtr mInputHandler;

        EventToken<Event::PauseEvent> mPauseToken;
        EventToken<Event::QuitEvent> mQuitToken;
        EventToken<Event::ApplicationEvent> mApplicationToken;
        EventToken<Event::SurfaceChangedEvent> mSurfaceChangedToken;
        EventToken<Event::ActivatedEvent> mActivatedToken;
        EventToken<event::TimeScaleEvent> mTimeScaleToken;
    };
}

