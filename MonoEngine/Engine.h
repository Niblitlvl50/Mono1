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
#include "Events/EventFwd.h"
#include "EventHandler/EventToken.h"

namespace System2
{
    class IWindow;
    class IInputHandler;
};

namespace mono
{
    class Engine
    {
    public:
        Engine(System2::IWindow* window, const ICameraPtr& camera, EventHandler& eventHandler);
        ~Engine();

        void Run(IZonePtr zone);

    private:

        bool OnPause(const event::PauseEvent& event);
        bool OnQuit(const event::QuitEvent& event);
        bool OnApplication(const event::ApplicationEvent& event);
        bool OnSurfaceChanged(const event::SurfaceChangedEvent& event);
        bool OnActivated(const event::ActivatedEvent& event);
        bool OnTimeScale(const event::TimeScaleEvent& event);

        bool mPause;
        bool mQuit;
        bool mUpdateLastTime;

        float mTimeScale;

        System2::IWindow* m_window;
        ICameraPtr mCamera;
        EventHandler& mEventHandler;
        std::shared_ptr<System2::IInputHandler> m_input_handler;

        EventToken<event::PauseEvent> mPauseToken;
        EventToken<event::QuitEvent> mQuitToken;
        EventToken<event::ApplicationEvent> mApplicationToken;
        EventToken<event::SurfaceChangedEvent> mSurfaceChangedToken;
        EventToken<event::ActivatedEvent> mActivatedToken;
        EventToken<event::TimeScaleEvent> mTimeScaleToken;
    };
}

