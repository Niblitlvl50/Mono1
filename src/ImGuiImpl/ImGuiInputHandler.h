
#pragma once

#include "MonoFwd.h"
#include "EventHandler/EventToken.h"
#include "Events/EventFwd.h"

class ImGuiInputHandler
{
public:

    ImGuiInputHandler(mono::EventHandler& event_handler);
    ~ImGuiInputHandler();

private:

    mono::EventResult OnKeyDown(const event::KeyDownEvent& event);
    mono::EventResult OnKeyUp(const event::KeyUpEvent& event);
    mono::EventResult OnTextInput(const event::TextInputEvent& event);
    mono::EventResult OnMouseDown(const event::MouseDownEvent& event);
    mono::EventResult OnMouseUp(const event::MouseUpEvent& event);
    mono::EventResult OnMouseMove(const event::MouseMotionEvent& event);
    mono::EventResult OnMouseWheel(const event::MouseWheelEvent& event);
    mono::EventResult OnMultiGesture(const event::MultiGestureEvent& event);

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
