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

Engine::Engine(System::IWindow* window, const ICameraPtr& camera, EventHandler& event_handler)
    : m_window(window),
      m_camera(camera),
      m_event_handler(event_handler)
{
    using namespace std::placeholders;

    const auto screen_to_world_func = std::bind(ScreenToWorld, _1, _2, m_window, m_camera);
    m_input_handler = std::make_unique<InputHandler>(screen_to_world_func, m_event_handler);

    const event::PauseEventFunc pauseFunc = std::bind(&Engine::OnPause, this, _1);
    const event::QuitEventFunc quitFunc = std::bind(&Engine::OnQuit, this, _1);
    const event::ApplicationEventFunc appFunc = std::bind(&Engine::OnApplication, this, _1);
    const event::SurfaceChangedEventFunc surfaceChangedFunc = std::bind(&Engine::OnSurfaceChanged, this, _1);
    const event::ActivatedEventFunc activatedFunc = std::bind(&Engine::OnActivated, this, _1);
    const event::TimeScaleEventFunc timeScaleFunc = std::bind(&Engine::OnTimeScale, this, _1);

    mPauseToken = m_event_handler.AddListener(pauseFunc);
    mQuitToken = m_event_handler.AddListener(quitFunc);
    mApplicationToken = m_event_handler.AddListener(appFunc);
    mSurfaceChangedToken = m_event_handler.AddListener(surfaceChangedFunc);
    mActivatedToken = m_event_handler.AddListener(activatedFunc);
    mTimeScaleToken = m_event_handler.AddListener(timeScaleFunc);
}

Engine::~Engine()
{
    m_event_handler.RemoveListener(mPauseToken);
    m_event_handler.RemoveListener(mQuitToken);
    m_event_handler.RemoveListener(mApplicationToken);
    m_event_handler.RemoveListener(mSurfaceChangedToken);
    m_event_handler.RemoveListener(mActivatedToken);
    m_event_handler.RemoveListener(mTimeScaleToken);
}

int Engine::Run(IZonePtr zone)
{
    zone->OnLoad(m_camera);

    Renderer renderer(m_camera);
    Updater updater;

    unsigned int last_time = System::GetMilliseconds();

    while(!m_quit)
    {
        // When exiting the application on iOS the lastTime variable
        // will be from when you exited, and then when you resume
        // the app the calculated delta will be huge and screw
        // everything up, thats why we need to update it here.
        if(m_update_last_time)
        {
            last_time = System::GetMilliseconds();
            m_update_last_time = false;
        }

        const unsigned int before_time = System::GetMilliseconds();
        const unsigned int delta = (before_time - last_time) * m_time_scale;

        // Handle input events
        System::ProcessSystemEvents(m_input_handler.get());
        if(!m_pause)
        {
            // Let the zone add stuff that will be rendered and updated
            zone->Accept(renderer);
            zone->Accept(updater);

            updater.AddUpdatable(m_camera);

            // Update all the stuff...
            updater.Update(delta);

            // Draw...
            m_window->MakeCurrent();
            renderer.DrawFrame();
            m_window->SwapBuffers();
        }

        last_time = before_time;

        // Sleep for a millisecond
        //System::Sleep(1);
    }

    // Remove possible follow entity and unload the zone
    m_camera->Unfollow();
    const int exit_code = zone->OnUnload();

    // Reset the quit, pause and updateLastTime flag for when you want
    // to reuse the engine for another zone.
    m_quit = false;
    m_pause = false;
    m_update_last_time = false;

    return exit_code;
}

bool Engine::OnPause(const event::PauseEvent& event)
{
    m_pause = event.pause;
    return true;
}

bool Engine::OnQuit(const event::QuitEvent&)
{
    m_quit = true;
    return false;
}

bool Engine::OnApplication(const event::ApplicationEvent& event)
{
    if(event.state == event::ApplicationState::ENTER_BACKGROUND)
    {
        m_pause = true;
    }
    else if(event.state == event::ApplicationState::ENTER_FOREGROUND)
    {
        m_pause = false;
        m_update_last_time = true;
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
    m_time_scale = event.time_scale;
    return false;
}
