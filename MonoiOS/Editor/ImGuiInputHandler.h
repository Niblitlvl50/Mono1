//
//  ImGuiInputHandler.hpp
//  MonoiOS
//
//  Created by Niklas Damberg on 24/06/16.
//
//

#pragma once

#include "MonoFwd.h"
#include "EventToken.h"
#include "EventFwd.h"

namespace editor
{
    class ImGuiInputHandler
    {
    public:

        ImGuiInputHandler(mono::EventHandler& event_handler);
        ~ImGuiInputHandler();

    private:

        bool OnKeyDown(const event::KeyDownEvent& event);
        bool OnKeyUp(const event::KeyUpEvent& event);
        bool OnTextInput(const event::TextInputEvent& event);
        bool OnMouseDown(const event::MouseDownEvent& event);
        bool OnMouseUp(const event::MouseUpEvent& event);
        bool OnMouseMove(const event::MouseMotionEvent& event);
        bool OnMouseWheel(const event::MouseWheelEvent& event);
        bool OnMultiGesture(const event::MultiGestureEvent& event);

        mono::EventHandler& m_eventHandler;
        mono::EventToken<event::KeyDownEvent> m_keyDownToken;
        mono::EventToken<event::KeyUpEvent> m_keyUpToken;
        mono::EventToken<event::TextInputEvent> m_textInputToken;
        mono::EventToken<event::MouseDownEvent> m_mouseDownToken;
        mono::EventToken<event::MouseUpEvent> m_mouseUpToken;
        mono::EventToken<event::MouseMotionEvent> m_mouseMoveToken;
        mono::EventToken<event::MouseWheelEvent> m_mouseWheelToken;
        mono::EventToken<event::MultiGestureEvent> m_multiGestureToken;
    };
}
