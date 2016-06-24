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

        void OnKeyDown(const event::KeyDownEvent& event);
        void OnKeyUp(const event::KeyUpEvent& event);
        void OnTextInput(const event::TextInputEvent& event);
        void OnMouseDown(const event::MouseDownEvent& event);
        void OnMouseUp(const event::MouseUpEvent& event);
        void OnMouseMove(const event::MouseMotionEvent& event);
        void OnMouseWheel(const event::MouseWheelEvent& event);

        mono::EventHandler& m_eventHandler;
        mono::EventToken<event::KeyDownEvent> m_keyDownToken;
        mono::EventToken<event::KeyUpEvent> m_keyUpToken;
        mono::EventToken<event::TextInputEvent> m_textInputToken;
        mono::EventToken<event::MouseDownEvent> m_mouseDownToken;
        mono::EventToken<event::MouseUpEvent> m_mouseUpToken;
        mono::EventToken<event::MouseMotionEvent> m_mouseMoveToken;
        mono::EventToken<event::MouseWheelEvent> m_mouseWheelToken;
    };
}
