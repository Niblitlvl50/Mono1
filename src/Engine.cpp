/*
 *  MonolithEngine.cpp
 *  Monolith1
 *
 *  Created by Niblit on 2011-02-07.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "Engine.h"
#include "SystemContext.h"
#include "InputHandler.h"
#include "Updater.h"

#include "Camera/ICamera.h"
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
        const System::Size& size = window->Size();

        const math::Vector window_size(size.width, size.height);
        const float ratio = window_size.x / window_size.y;

        math::Quad viewport = camera->GetViewport();
        viewport.mB.y = viewport.mB.x / ratio;
        
        const math::Vector& scale = viewport.mB / window_size;
        
        const float screen_x = x;
        const float screen_y = window_size.y - y;
        
        const float temp_x = screen_x * scale.x;
        const float temp_y = screen_y * scale.y;
        
        x = temp_x + viewport.mA.x;
        y = temp_y + viewport.mA.y;
    }
}

Engine::Engine(System::IWindow* window, const ICameraPtr& camera, SystemContext* system_context, EventHandler& event_handler)
    : m_window(window)
    , m_camera(camera)
    , m_system_context(system_context)
    , m_event_handler(event_handler)
{
    using namespace std::placeholders;

    const auto screen_to_world_func = std::bind(ScreenToWorld, _1, _2, m_window, m_camera);
    m_input_handler = std::make_unique<InputHandler>(screen_to_world_func, m_event_handler);

    const event::PauseEventFunc pause_func = std::bind(&Engine::OnPause, this, _1);
    const event::QuitEventFunc quit_func = std::bind(&Engine::OnQuit, this, _1);
    const event::ApplicationEventFunc app_func = std::bind(&Engine::OnApplication, this, _1);
    const event::SurfaceChangedEventFunc surface_changed_func = std::bind(&Engine::OnSurfaceChanged, this, _1);
    const event::ActivatedEventFunc activated_func = std::bind(&Engine::OnActivated, this, _1);
    const event::TimeScaleEventFunc time_scale_func = std::bind(&Engine::OnTimeScale, this, _1);

    m_pause_token = m_event_handler.AddListener(pause_func);
    m_quit_token = m_event_handler.AddListener(quit_func);
    m_application_token = m_event_handler.AddListener(app_func);
    m_surface_changed_token = m_event_handler.AddListener(surface_changed_func);
    m_activated_token = m_event_handler.AddListener(activated_func);
    m_time_scale_token = m_event_handler.AddListener(time_scale_func);
}

Engine::~Engine()
{
    m_event_handler.RemoveListener(m_pause_token);
    m_event_handler.RemoveListener(m_quit_token);
    m_event_handler.RemoveListener(m_application_token);
    m_event_handler.RemoveListener(m_surface_changed_token);
    m_event_handler.RemoveListener(m_activated_token);
    m_event_handler.RemoveListener(m_time_scale_token);
}

int Engine::Run(IZonePtr zone)
{
    zone->OnLoad(m_camera);

    Renderer renderer;

    UpdateContext update_context;
    update_context.frame_count = 0;
    update_context.delta_ms = 0;
    update_context.total_time = 0;

    Updater updater;

    uint32_t last_time = System::GetMilliseconds();

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

        const uint32_t before_time = System::GetMilliseconds();
        const uint32_t delta_ms = (before_time - last_time) * m_time_scale;
        update_context.total_time += delta_ms;

        // Handle input events
        System::ProcessSystemEvents(m_input_handler.get());
        
        const System::Size& window_size = m_window->Size();
        renderer.SetWindowSize(math::Vector(window_size.width, window_size.height));

        const math::Quad& viewport = m_camera->GetViewport();
        const math::Quad camera_quad(viewport.mA, viewport.mA + viewport.mB);
        renderer.SetViewport(camera_quad);

        if(!m_pause)
        {
            update_context.frame_count++;
            update_context.delta_ms = delta_ms;

            m_window->MakeCurrent();
            m_system_context->Update(update_context);

            // Update all the stuff...
            zone->Accept(updater);
            updater.AddUpdatable(m_camera.get());
            updater.Update(update_context);

            // Draw...
            zone->Accept(renderer);
            renderer.DrawFrame();

            m_window->SwapBuffers();
            m_system_context->Sync();
        }

        last_time = before_time;

        // Sleep for a millisecond
        System::Sleep(1);
    }

    // Remove possible follow entity and unload the zone
    m_camera->Unfollow();
    const int exit_code = zone->OnUnload();

    // Reset the quit, pause and m_update_last_time flag for when you want
    // to reuse the engine for another zone.
    m_quit = false;
    m_pause = false;
    m_update_last_time = false;
    m_time_scale = 1.0f;

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
