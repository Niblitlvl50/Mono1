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
#include "Rendering/RenderPtrFwd.h"

namespace System
{
    class IWindow;
    class IInputHandler;
};

namespace mono
{
    class Engine
    {
    public:
        Engine(System::IWindow* window, const ICameraPtr& camera, float pixels_per_meter, EventHandler& event_handler);
        ~Engine();

        int Run(IZonePtr zone);

    private:

        bool OnPause(const event::PauseEvent& event);
        bool OnQuit(const event::QuitEvent& event);
        bool OnApplication(const event::ApplicationEvent& event);
        bool OnSurfaceChanged(const event::SurfaceChangedEvent& event);
        bool OnActivated(const event::ActivatedEvent& event);
        bool OnTimeScale(const event::TimeScaleEvent& event);

        bool m_pause = false;
        bool m_quit = false;
        bool m_update_last_time = false;
        float m_time_scale = 1.0f;

        System::IWindow* m_window;
        ICameraPtr m_camera;
        float m_pixels_per_meter;
        EventHandler& m_event_handler;
        std::unique_ptr<System::IInputHandler> m_input_handler;

        EventToken<event::PauseEvent> m_pause_token;
        EventToken<event::QuitEvent> m_quit_token;
        EventToken<event::ApplicationEvent> m_application_token;
        EventToken<event::SurfaceChangedEvent> m_surface_changed_token;
        EventToken<event::ActivatedEvent> m_activated_token;
        EventToken<event::TimeScaleEvent> m_time_scale_token;
    };
}

