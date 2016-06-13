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
		
        void OnPause(const event::PauseEvent& event);
        void OnQuit(const event::QuitEvent& event);
        void OnApplication(const event::ApplicationEvent& event);
        void OnSurfaceChanged(const event::SurfaceChangedEvent& event);
        void OnActivated(const event::ActivatedEvent& event);
        void OnTimeScale(const event::TimeScaleEvent& event);

        bool mPause;
        bool mQuit;
        bool mUpdateLastTime;

        float mTimeScale;

        IWindowPtr mWindow;
        ICameraPtr mCamera;
        EventHandler& mEventHandler;
        IInputHandlerPtr mInputHandler;

        EventToken<event::PauseEvent> mPauseToken;
        EventToken<event::QuitEvent> mQuitToken;
        EventToken<event::ApplicationEvent> mApplicationToken;
        EventToken<event::SurfaceChangedEvent> mSurfaceChangedToken;
        EventToken<event::ActivatedEvent> mActivatedToken;
        EventToken<event::TimeScaleEvent> mTimeScaleToken;
    };
}

