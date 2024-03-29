
#include "ImGuiInputHandler.h"
#include "Events/EventFuncFwd.h"
#include "EventHandler/EventHandler.h"

#include "System/Keycodes.h"
#include "System/System.h"

#include "Events/KeyEvent.h"
#include "Events/TextInputEvent.h"
#include "Events/MouseEvent.h"
#include "Events/MultiGestureEvent.h"

#include "imgui/imgui.h"

ImGuiInputHandler::ImGuiInputHandler(mono::EventHandler& event_handler)
    : m_eventHandler(event_handler)
{
    using namespace std::placeholders;

    event::KeyDownEventFunc key_down = std::bind(&ImGuiInputHandler::OnKeyDown, this, _1);
    event::KeyUpEventFunc key_up = std::bind(&ImGuiInputHandler::OnKeyUp, this, _1);
    event::TextInputEventFunc text_input = std::bind(&ImGuiInputHandler::OnTextInput, this, _1);
    event::MouseDownEventFunc mouse_down = std::bind(&ImGuiInputHandler::OnMouseDown, this, _1);
    event::MouseUpEventFunc mouse_up = std::bind(&ImGuiInputHandler::OnMouseUp, this, _1);
    event::MouseMotionEventFunc mouse_move = std::bind(&ImGuiInputHandler::OnMouseMove, this, _1);
    event::MouseWheelEventFunc mouse_wheel = std::bind(&ImGuiInputHandler::OnMouseWheel, this, _1);
    event::MultiGestureEventFunc multi_gesture = std::bind(&ImGuiInputHandler::OnMultiGesture, this, _1);

    m_keyDownToken = m_eventHandler.AddListener(key_down);
    m_keyUpToken = m_eventHandler.AddListener(key_up);
    m_textInputToken = m_eventHandler.AddListener(text_input);
    m_mouseDownToken = m_eventHandler.AddListener(mouse_down);
    m_mouseUpToken = m_eventHandler.AddListener(mouse_up);
    m_mouseMoveToken = m_eventHandler.AddListener(mouse_move);
    m_mouseWheelToken = m_eventHandler.AddListener(mouse_wheel);
    m_multiGestureToken = m_eventHandler.AddListener(multi_gesture);

    ImGuiIO& io = ImGui::GetIO();
    io.KeyMap[ImGuiKey_Tab] = System::KeycodeToNative(Keycode::TAB);
    io.KeyMap[ImGuiKey_LeftArrow] = System::KeycodeToNative(Keycode::LEFT);
    io.KeyMap[ImGuiKey_RightArrow] = System::KeycodeToNative(Keycode::RIGHT);
    io.KeyMap[ImGuiKey_UpArrow] = System::KeycodeToNative(Keycode::UP);
    io.KeyMap[ImGuiKey_DownArrow] = System::KeycodeToNative(Keycode::DOWN);
    //io.KeyMap[ImGuiKey_PageUp] = SDL_SCANCODE_PAGEUP;
    //io.KeyMap[ImGuiKey_PageDown] = SDL_SCANCODE_PAGEDOWN;
    //io.KeyMap[ImGuiKey_Home] = SDL_SCANCODE_HOME;
    //io.KeyMap[ImGuiKey_End] = SDL_SCANCODE_END;
    io.KeyMap[ImGuiKey_Delete] = System::KeycodeToNative(Keycode::DELETE);
    io.KeyMap[ImGuiKey_Backspace] = System::KeycodeToNative(Keycode::BACKSPACE);
    io.KeyMap[ImGuiKey_Enter] = System::KeycodeToNative(Keycode::ENTER);
    io.KeyMap[ImGuiKey_C] = System::KeycodeToNative(Keycode::C);
    io.KeyMap[ImGuiKey_V] = System::KeycodeToNative(Keycode::V);
    //io.KeyMap[ImGuiKey_Escape] = SDLK_ESCAPE;

    io.GetClipboardTextFn = [](void* user_data) -> const char* {
        static char text_buffer[1024];
        const uint32_t n_chars = System::GetClipboardText(text_buffer, std::size(text_buffer));
        text_buffer[n_chars] = '\0';
        return text_buffer;
    };
}

ImGuiInputHandler::~ImGuiInputHandler()
{
    m_eventHandler.RemoveListener(m_keyDownToken);
    m_eventHandler.RemoveListener(m_keyUpToken);
    m_eventHandler.RemoveListener(m_textInputToken);
    m_eventHandler.RemoveListener(m_mouseDownToken);
    m_eventHandler.RemoveListener(m_mouseUpToken);
    m_eventHandler.RemoveListener(m_mouseMoveToken);
    m_eventHandler.RemoveListener(m_mouseWheelToken);
    m_eventHandler.RemoveListener(m_multiGestureToken);
}

mono::EventResult ImGuiInputHandler::OnKeyDown(const event::KeyDownEvent& event)
{
    const int key = System::KeycodeToNative(event.key);
    if(key >= 512)
        return mono::EventResult::PASS_ON;

    ImGui::GetIO().KeysDown[key] = true;
    ImGui::GetIO().KeyCtrl = event.ctrl;
    ImGui::GetIO().KeyShift = event.shift;
    ImGui::GetIO().KeyAlt = event.alt;
    ImGui::GetIO().KeySuper = event.super;
    return ImGui::GetIO().WantCaptureKeyboard ? mono::EventResult::HANDLED : mono::EventResult::PASS_ON;
}

mono::EventResult ImGuiInputHandler::OnKeyUp(const event::KeyUpEvent& event)
{
    const int key = System::KeycodeToNative(event.key);
    if(key >= 512)
        return mono::EventResult::PASS_ON;

    ImGui::GetIO().KeysDown[key] = false;
    ImGui::GetIO().KeyCtrl = event.ctrl;
    ImGui::GetIO().KeyShift = event.shift;
    ImGui::GetIO().KeyAlt = event.alt;
    ImGui::GetIO().KeySuper = event.super;
    return ImGui::GetIO().WantCaptureKeyboard ? mono::EventResult::HANDLED : mono::EventResult::PASS_ON;
}

mono::EventResult ImGuiInputHandler::OnTextInput(const event::TextInputEvent& event)
{
    ImGui::GetIO().AddInputCharactersUTF8(event.text);
    return ImGui::GetIO().WantTextInput ? mono::EventResult::HANDLED : mono::EventResult::PASS_ON;
}

mono::EventResult ImGuiInputHandler::OnMouseDown(const event::MouseDownEvent& event)
{
    if(event.key != MouseButton::LEFT && event.key != MouseButton::RIGHT)
        return mono::EventResult::PASS_ON;

    if(event.key == MouseButton::LEFT)
        ImGui::GetIO().MouseDown[0] = true;
    else if(event.key == MouseButton::RIGHT)
        ImGui::GetIO().MouseDown[1] = true;

    return ImGui::GetIO().WantCaptureMouse ? mono::EventResult::HANDLED : mono::EventResult::PASS_ON;
}

mono::EventResult ImGuiInputHandler::OnMouseUp(const event::MouseUpEvent& event)
{
    if(event.key != MouseButton::LEFT && event.key != MouseButton::RIGHT)
        return mono::EventResult::PASS_ON;

    if(event.key == MouseButton::LEFT)
        ImGui::GetIO().MouseDown[0] = false;
    else if(event.key == MouseButton::RIGHT)
        ImGui::GetIO().MouseDown[1] = false;

    return ImGui::GetIO().WantCaptureMouse ? mono::EventResult::HANDLED : mono::EventResult::PASS_ON;
}

mono::EventResult ImGuiInputHandler::OnMouseMove(const event::MouseMotionEvent& event)
{
    ImGui::GetIO().MousePos = ImVec2(event.screen_x, event.screen_y);
    return ImGui::GetIO().WantCaptureMouse ? mono::EventResult::HANDLED : mono::EventResult::PASS_ON;
}

mono::EventResult ImGuiInputHandler::OnMouseWheel(const event::MouseWheelEvent& event)
{
    ImGui::GetIO().MouseWheel = event.y;
    return ImGui::GetIO().WantCaptureMouse ? mono::EventResult::HANDLED : mono::EventResult::PASS_ON;
}

mono::EventResult ImGuiInputHandler::OnMultiGesture(const event::MultiGestureEvent& event)
{
    return mono::EventResult::PASS_ON;
    //return ImGui::GetIO().WantCaptureMouse;
}


