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

namespace System
{
    class IWindow;
};

namespace mono
{
    class Engine
    {
    public:
        Engine(System::IWindow* window, SystemContext* system_context, EventHandler* event_handler);
        ~Engine();

        int Run(IZone* zone);

    private:

        mono::EventResult OnPause(const event::PauseEvent& event);
        mono::EventResult OnQuit(const event::QuitEvent& event);
        mono::EventResult OnApplication(const event::ApplicationEvent& event);
        mono::EventResult OnSurfaceChanged(const event::SurfaceChangedEvent& event);
        mono::EventResult OnActivated(const event::ActivatedEvent& event);
        mono::EventResult OnTimeScale(const event::TimeScaleEvent& event);

        bool m_pause = false;
        bool m_quit = false;
        bool m_update_last_time = false;
        float m_time_scale = 1.0f;

        System::IWindow* m_window;
        SystemContext* m_system_context;
        EventHandler* m_event_handler;

        EventToken<event::PauseEvent> m_pause_token;
        EventToken<event::QuitEvent> m_quit_token;
        EventToken<event::ApplicationEvent> m_application_token;
        EventToken<event::SurfaceChangedEvent> m_surface_changed_token;
        EventToken<event::ActivatedEvent> m_activated_token;
        EventToken<event::TimeScaleEvent> m_time_scale_token;
    };
}

