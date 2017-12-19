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
#include "Updater.h"

#include "Rendering/ICamera.h"
#include "Zone/IZone.h"

#include "System/System.h"

#include "EventHandler/EventHandler.h"
#include "Events/EventFuncFwd.h"
#include "Events/PauseEvent.h"
#include "Events/QuitEvent.h"
#include "Events/ApplicationEvent.h"
#include "Events/SurfaceChangedEvent.h"
#include "Events/ActivatedEvent.h"
#include "Events/TimeScaleEvent.h"

#include "Rendering/Renderer.h"

#include "Math/Vector.h"
#include "Math/Quad.h"

using namespace mono;

namespace
{
    void ScreenToWorld(float& x, float& y, System::IWindow* window, ICameraPtr camera)
    {
        const System::Size& window_size = window->Size();
        const math::Vector size(window_size.width, window_size.height);

        const math::Quad& viewport = camera->GetViewport();
        
        const math::Vector& scale = viewport.mB / size;
        
        const float screen_x = x;
        const float screen_y = size.y - y;
        
        const float temp_x = screen_x * scale.x;
        const float temp_y = screen_y * scale.y;
        
        x = temp_x + viewport.mA.x;
        y = temp_y + viewport.mA.y;
    }
}

Engine::Engine(System::IWindow* window, const ICameraPtr& camera, EventHandler& eventHandler)
    : m_window(window),
      mCamera(camera),
      mEventHandler(eventHandler)
{
    using namespace std::placeholders;

    const auto func = std::bind(ScreenToWorld, _1, _2, m_window, mCamera);
    m_input_handler = std::make_shared<InputHandler>(func, mEventHandler);

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

int Engine::Run(IZonePtr zone)
{
    zone->OnLoad(mCamera);

    Renderer renderer(mCamera);
    Updater updater;

    unsigned int lastTime = System::GetMilliseconds();

    while(!mQuit)
    {
        // When exiting the application on iOS the lastTime variable
        // will be from when you exited, and then when you resume
        // the app the calculated delta will be huge and screw
        // everything up, thats why we need to update it here.
        if(mUpdateLastTime)
        {
            lastTime = System::GetMilliseconds();
            mUpdateLastTime = false;
        }

        const unsigned int beforeTime = System::GetMilliseconds();
        const unsigned int delta = (beforeTime - lastTime) * mTimeScale;

        // Handle input events
        System::ProcessSystemEvents(m_input_handler.get());
        if(!mPause)
        {
            // Let the zone add stuff that will be rendered and updated
            zone->Accept(renderer);
            zone->Accept(updater);

            updater.AddUpdatable(mCamera);

            // Update all the stuff...
            updater.Update(delta);

            // Draw...
            m_window->MakeCurrent();
            renderer.DrawFrame();
            m_window->SwapBuffers();
        }

        lastTime = beforeTime;

        // Sleep for a millisecond
        //System::Sleep(1);
    }

    // Remove possible follow entity and unload the zone
    mCamera->Unfollow();
    zone->OnUnload();

    // Reset the quit, pause and updateLastTime flag for when you want
    // to reuse the engine for another zone.
    mQuit = false;
    mPause = false;
    mUpdateLastTime = false;

    return zone->ExitCode();
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
    m_window->SurfaceChanged(event.width, event.height);
    return false;
}

bool Engine::OnActivated(const event::ActivatedEvent& event)
{
    //m_window->Activated(event.gain);
    return false;
}

bool Engine::OnTimeScale(const event::TimeScaleEvent& event)
{
    mTimeScale = event.time_scale;
    return false;
}
