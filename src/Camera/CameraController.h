
#pragma once

#include "MonoFwd.h"
#include "Events/EventFwd.h"
#include "EventHandler/EventToken.h"
#include "Math/Vector.h"

namespace mono
{
    class MouseCameraController
    {
    public:
        
        //MouseCameraController(int window_width, int window_height, ICamera* camera, EventHandler& event_handler);
        MouseCameraController(ICamera* camera, EventHandler& event_handler);
        ~MouseCameraController();

        void Enable();
        void Disable();
        
    private:
        
        mono::EventResult OnMouseDown(const event::MouseDownEvent& event);
        mono::EventResult OnMouseUp(const event::MouseUpEvent& event);
        mono::EventResult OnMouseMove(const event::MouseMotionEvent& event);
        mono::EventResult OnMultiGesture(const event::MultiGestureEvent& event);
        mono::EventResult OnMouseWheel(const event::MouseWheelEvent& event);
        
        int m_window_width;
        int m_window_height;
        ICamera* m_camera;
        EventHandler& m_event_handler;
        bool m_enabled;

        bool m_translate;
        math::Vector m_translate_delta;

        mono::EventToken<event::MouseDownEvent> m_mouse_down_token;
        mono::EventToken<event::MouseUpEvent> m_mouse_up_token;
        mono::EventToken<event::MouseMotionEvent> m_mouse_move_token;
        mono::EventToken<event::MultiGestureEvent> m_multi_gesture_token;
        mono::EventToken<event::MouseWheelEvent> m_mouse_wheel_token;
    };
}
