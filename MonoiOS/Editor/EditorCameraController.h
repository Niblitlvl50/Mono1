//
//  EditorCameraController.hpp
//  MonoiOS
//
//  Created by Niklas Damberg on 26/06/16.
//
//

#pragma once

#include "MonoPtrFwd.h"
#include "EventFwd.h"
#include "EventToken.h"
#include "Vector2f.h"

namespace editor
{
    class CameraController
    {
    public:

        CameraController(const mono::ICameraPtr& camera, const math::Vector2f& window_size, mono::EventHandler& event_handler);
        ~CameraController();

    private:

        bool OnMouseDown(const event::MouseDownEvent& event);
        bool OnMouseUp(const event::MouseUpEvent& event);
        bool OnMouseMove(const event::MouseMotionEvent& event);
        bool OnMultiGesture(const event::MultiGestureEvent& event);

        mono::ICameraPtr m_camera;
        const math::Vector2f m_windowSize;
        mono::EventHandler& m_eventHandler;

        bool m_translate;
        math::Vector2f m_translateDelta;

        mono::EventToken<event::MouseUpEvent> m_mouseUpToken;
        mono::EventToken<event::MouseDownEvent> m_mouseDownToken;
        mono::EventToken<event::MouseMotionEvent> m_mouseMoveToken;
        mono::EventToken<event::MultiGestureEvent> m_multiGestureToken;
    };
}

