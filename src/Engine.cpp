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
#include "Input/InputHandler.h"
#include "IUpdater.h"

#include "Camera/ICamera.h"
#include "Zone/IZone.h"

#include "System/Audio.h"
#include "System/System.h"

#include "EventHandler/EventHandler.h"
#include "Events/EventFuncFwd.h"
#include "Events/KeyEvent.h"
#include "Events/PauseEvent.h"
#include "Events/QuitEvent.h"
#include "Events/ApplicationEvent.h"
#include "Events/ScreenEvent.h"
#include "Events/SurfaceChangedEvent.h"
#include "Events/TimeScaleEvent.h"

#include "Rendering/RendererSokol.h"

#include "Math/Vector.h"
#include "Math/Quad.h"

namespace
{
    class Updater : public mono::IUpdater
    {
    public:

        void AddUpdatable(mono::IUpdatable* updatable) override
        {
            m_updatables.push_back(updatable);
        }

        void Update(const mono::UpdateContext& update_context)
        {
            for(mono::IUpdatable* updatable : m_updatables)
                updatable->Update(update_context);

            m_updatables.clear();
        }

        std::vector<mono::IUpdatable*> m_updatables;
    };
}

using namespace mono;

Engine::Engine(System::IWindow* window, ICamera* camera, SystemContext* system_context, EventHandler* event_handler)
    : m_window(window)
    , m_camera(camera)
    , m_system_context(system_context)
    , m_event_handler(event_handler)
{
    using namespace std::placeholders;

    const event::KeyDownEventFunc key_down_func = std::bind(&Engine::OnKeyDown, this, _1);
    const event::PauseEventFunc pause_func = std::bind(&Engine::OnPause, this, _1);
    const event::QuitEventFunc quit_func = std::bind(&Engine::OnQuit, this, _1);
    const event::ApplicationEventFunc app_func = std::bind(&Engine::OnApplication, this, _1);
    const event::ScreenEventFunc screen_func = std::bind(&Engine::OnScreenEvent, this, _1);
    const event::TimeScaleEventFunc time_scale_func = std::bind(&Engine::OnTimeScale, this, _1);

    m_key_down_token = m_event_handler->AddListener(key_down_func);
    m_pause_token = m_event_handler->AddListener(pause_func);
    m_quit_token = m_event_handler->AddListener(quit_func);
    m_application_token = m_event_handler->AddListener(app_func);
    m_screen_token = m_event_handler->AddListener(screen_func);
    m_time_scale_token = m_event_handler->AddListener(time_scale_func);
}

Engine::~Engine()
{
    m_event_handler->RemoveListener(m_key_down_token);
    m_event_handler->RemoveListener(m_pause_token);
    m_event_handler->RemoveListener(m_quit_token);
    m_event_handler->RemoveListener(m_application_token);
    m_event_handler->RemoveListener(m_screen_token);
    m_event_handler->RemoveListener(m_time_scale_token);
}

int Engine::Run(IZone* zone)
{
    RendererSokol renderer;

    const ScreenToWorldFunc screen_to_world_func = [this](float& x, float& y) {
        const math::Vector world = m_camera->ScreenToWorld(math::Vector(x, y));
        x = world.x;
        y = world.y;
    };

    InputHandler input_handler(screen_to_world_func, m_event_handler);
    UpdateContext update_context = { 0, 0, 0, 0.0f, 0.0f, false };
    Updater updater;

    zone->OnLoad(m_camera, &renderer);
    m_system_context->BeginSystems();

    uint32_t last_time = System::GetMilliseconds();

    while(!m_quit)
    {
        // When exiting the application on iOS the 'last_time' variable
        // will be from when you exited, and then when you resume
        // the app the calculated delta will be huge and screw
        // everything up, thats why we need to update it here.
        if(m_update_last_time)
        {
            last_time = System::GetMilliseconds();
            m_update_last_time = false;
        }

        const bool slow_down = System::IsKeyDown(Keycode::F1);
        const float slow_down_multiplier = slow_down ? 0.25f : 1.0f;

        const uint32_t before_time = System::GetMilliseconds();
        const uint32_t delta_ms = before_time - last_time;
        const uint32_t delta_ms_timescaled =
            std::clamp(
                uint32_t(delta_ms * m_time_scale * slow_down_multiplier),
                1u, std::numeric_limits<uint32_t>::max());

        update_context.frame_count++;
        update_context.delta_ms = delta_ms_timescaled;
        update_context.delta_s = float(delta_ms_timescaled) / 1000.0f;
        update_context.delta_s_raw = float(delta_ms) / 1000.0f;
        update_context.paused = m_pause;
        update_context.timestamp += delta_ms_timescaled;

        const System::Size size = m_window->Size();
        const System::Size drawable_size = m_window->DrawableSize();
        const math::Vector window_size(size.width, size.height);

        m_camera->SetWindowSize(window_size);

        renderer.SetWindowSize(window_size);
        renderer.SetDrawableSize(math::Vector(drawable_size.width, drawable_size.height));
        renderer.SetViewport(m_camera->GetViewport());

        // Handle input events
        System::ProcessSystemEvents(&input_handler);

        if(!m_suspended)
        {
            audio::MixSounds();

            renderer.SetDeltaAndTimestamp(update_context.delta_s, update_context.timestamp);

            m_window->MakeCurrent();
            m_system_context->Update(update_context);

            // Update all the stuff...
            zone->Accept(updater);
            updater.AddUpdatable(m_camera);
            updater.Update(update_context);

            // Draw...
            zone->Accept(renderer);
            renderer.DrawFrame();

            m_window->SwapBuffers();

            zone->PostUpdate();
            m_system_context->SyncSystems(m_pause);
        }

        /*
        const uint32_t after_time = System::GetMilliseconds();
        const uint32_t delta_time = after_time - before_time;

        // 60 fps
        const float sleep_time = (1.0f / 60.0f) - (float(delta_time) / 1000.0f);
        //printf("sleep_tiem: %f | %f | %u\n", sleep_time, (float(delta_time) / 1000.0f), uint32_t(sleep_time * 1000.0f));

        last_time = before_time;

        if(sleep_time > 0.0f)
        {
            System::Sleep(uint32_t(sleep_time * 1000.0f));
        }
        */

        last_time = before_time;

        // Sleep for a millisecond, this highly reduces fps
        System::Sleep(1);
    }

    // Unload the zone and sync the game systems, this is mostly to clean up entities.
    const int exit_code = zone->OnUnload();
    m_system_context->ResetSystems();

    // Reset the quit, pause and m_update_last_time flag for when you want
    // to reuse the engine for another zone.
    m_quit = false;
    m_pause = false;
    m_suspended = false;
    m_update_last_time = false;
    m_time_scale = 1.0f;

    return exit_code;
}

mono::EventResult Engine::OnKeyDown(const event::KeyDownEvent& event)
{
    const bool toggle_fullscreen = (event.key == Keycode::ENTER && event.alt);
    if(toggle_fullscreen)
    {
        if(m_fullscreen)
            m_window->SetWindowed();
        else
            m_window->SetFullscreen(System::FullscreenMode::WINDOWED);

        m_fullscreen = !m_fullscreen;
    }

    return toggle_fullscreen ? mono::EventResult::HANDLED : mono::EventResult::PASS_ON;
}

mono::EventResult Engine::OnPause(const event::PauseEvent& event)
{
    m_pause = event.pause;
    return mono::EventResult::PASS_ON;
}

mono::EventResult Engine::OnQuit(const event::QuitEvent&)
{
    m_quit = true;
    return mono::EventResult::PASS_ON;
}

mono::EventResult Engine::OnApplication(const event::ApplicationEvent& event)
{
    if(event.state == event::ApplicationState::ENTER_BACKGROUND)
    {
        m_suspended = true;
    }
    else if(event.state == event::ApplicationState::ENTER_FOREGROUND)
    {
        m_suspended = false;
        m_update_last_time = true;
    }

    return mono::EventResult::PASS_ON;
}

mono::EventResult Engine::OnScreenEvent(const event::ScreenEvent& event)
{
    switch(event.screen_mode)
    {
    case event::ScreenMode::WINDOWED:
        m_window->SetWindowed();
        break;
    case event::ScreenMode::FULLSCREEN:
        m_window->SetFullscreen(System::FullscreenMode::FULLSCREEN);
        break;
    case event::ScreenMode::FULLSCREEN_DESKTOP:
        m_window->SetFullscreen(System::FullscreenMode::WINDOWED);
        break;
    }

    return mono::EventResult::PASS_ON;
}

mono::EventResult Engine::OnTimeScale(const event::TimeScaleEvent& event)
{
    m_time_scale = event.time_scale;
    return mono::EventResult::PASS_ON;
}
