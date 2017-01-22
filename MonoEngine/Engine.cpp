/*
 *  MonolithEngine.cpp
 *  Monolith1
 *
 *  Created by Niblit on 2011-02-07.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "Engine.h"
#include "InputHandler.h"

#include "IWindow.h"
#include "ICamera.h"
#include "IZone.h"

#include "System/SysTime.h"
#include "System/SysEvents.h"

#include "EventHandler/EventHandler.h"
#include "Events/EventFuncFwd.h"
#include "Events/PauseEvent.h"
#include "Events/QuitEvent.h"
#include "Events/ApplicationEvent.h"
#include "Events/SurfaceChangedEvent.h"
#include "Events/ActivatedEvent.h"
#include "Events/TimeScaleEvent.h"

#include "Renderer.h"

#include "Math/Vector.h"
#include "Math/Quad.h"

using namespace mono;

namespace
{
    void ScreenToWorld(float& x, float& y, IWindowPtr window, ICameraPtr camera)
    {
        const math::Vector& windowSize = window->Size();
        const math::Quad& viewport = camera->GetViewport();
        
        const math::Vector& scale = viewport.mB / windowSize;
        
        const float screenX = x;
        const float screenY = windowSize.y - y;
        
        const float tempx = screenX * scale.x;
        const float tempy = screenY * scale.y;
        
        x = tempx + viewport.mA.x;
        y = tempy + viewport.mA.y;
    }
}

Engine::Engine(const IWindowPtr& window, const ICameraPtr& camera, EventHandler& eventHandler)
    : mPause(false),
      mQuit(false),
      mUpdateLastTime(false),
      mTimeScale(1.0f),
      mWindow(window),
      mCamera(camera),
      mEventHandler(eventHandler)
{
    using namespace std::placeholders;

    const auto func = std::bind(ScreenToWorld, _1, _2, mWindow, mCamera);
    mInputHandler = std::make_shared<InputHandler>(func, mEventHandler);

    const event::PauseEventFunc pauseFunc = std::bind(&Engine::OnPause, this, _1);
    const event::QuitEventFunc quitFunc = std::bind(&Engine::OnQuit, this, _1);
    const event::ApplicationEventFunc appFunc = std::bind(&Engine::OnApplication, this, _1);
    const event::SurfaceChangedEventFunc surfaceChangedFunc = std::bind(&Engine::OnSurfaceChanged, this, _1);
    const event::ActivatedEventFunc activatedFunc = std::bind(&Engine::OnActivated, this, _1);
    const event::TimeScaleEventFunc timeScaleFunc = std::bind(&Engine::OnTimeScale, this, _1);

    mPauseToken = mEventHandler.AddListener(pauseFunc);
    mQuitToken = mEventHandler.AddListener(quitFunc);
    mApplicationToken = mEventHandler.AddListener(appFunc);
    mSurfaceChangedToken = mEventHandler.AddListener(surfaceChangedFunc);
    mActivatedToken = mEventHandler.AddListener(activatedFunc);
    mTimeScaleToken = mEventHandler.AddListener(timeScaleFunc);
}

Engine::~Engine()
{
    mEventHandler.RemoveListener(mPauseToken);
    mEventHandler.RemoveListener(mQuitToken);
    mEventHandler.RemoveListener(mApplicationToken);
    mEventHandler.RemoveListener(mSurfaceChangedToken);
    mEventHandler.RemoveListener(mActivatedToken);
    mEventHandler.RemoveListener(mTimeScaleToken);
}

void Engine::Run(IZonePtr zone)
{
    zone->OnLoad(mCamera);

    Renderer renderer(mCamera, mWindow);
    unsigned int lastTime = Time::GetMilliseconds();

    while(!mQuit)
    {
        // When exiting the application on iOS the lastTime variable
        // will be from when you exited, and then when you resume
        // the app the calculated delta will be huge and screw
        // everything up, thats why we need to update it here.
        if(mUpdateLastTime)
        {
            lastTime = Time::GetMilliseconds();
            mUpdateLastTime = false;
        }

        const unsigned int beforeTime = Time::GetMilliseconds();
        const unsigned int delta = (beforeTime - lastTime) * mTimeScale;

        // Handle input events
        Events::ProcessSystemEvents(mInputHandler);
        if(!mPause)
        {
            // Let the zone add stuff that will be rendered and updated
            zone->Accept(renderer);

            // Update all the stuff, and draw complete frame
            renderer.Update(delta);
            renderer.DrawFrame();
        }

        lastTime = beforeTime;

        // Sleep for a millisecond
        //Time::Sleep(1);
    }

    // Remove possible follow entity and unload the zone
    mCamera->Unfollow();
    zone->OnUnload();

    // Reset the quit, pause and updateLastTime flag for when you want
    // to reuse the engine for another zone.
    mQuit = false;
    mPause = false;
    mUpdateLastTime = false;
}

bool Engine::OnPause(const event::PauseEvent& event)
{
    mPause = event.pause;
    return true;
}

bool Engine::OnQuit(const event::QuitEvent&)
{
    mQuit = true;
    return false;
}

bool Engine::OnApplication(const event::ApplicationEvent& event)
{
    if(event.state == event::ApplicationState::ENTER_BACKGROUND)
    {
        mPause = true;
    }
    else if(event.state == event::ApplicationState::ENTER_FOREGROUND)
    {
        mPause = false;
        mUpdateLastTime = true;
    }

    return false;
}

bool Engine::OnSurfaceChanged(const event::SurfaceChangedEvent& event)
{
    mWindow->SurfaceChanged(event.width, event.height);
    return false;
}

bool Engine::OnActivated(const event::ActivatedEvent& event)
{
    mWindow->Activated(event.gain);
    return false;
}

bool Engine::OnTimeScale(const event::TimeScaleEvent& event)
{
    mTimeScale = event.time_scale;
    return false;
}
