//
//  ImGuiInputHandler.cpp
//  MonoiOS
//
//  Created by Niklas Damberg on 24/06/16.
//
//

#include "ImGuiInputHandler.h"
#include "Events/EventFuncFwd.h"
#include "EventHandler/EventHandler.h"

#include "System/SysKeycodes.h"

#include "Events/KeyUpEvent.h"
#include "Events/KeyDownEvent.h"
#include "Events/TextInputEvent.h"
#include "Events/MouseDownEvent.h"
#include "Events/MouseUpEvent.h"
#include "Events/MouseMotionEvent.h"
#include "Events/MouseWheelEvent.h"
#include "Events/MultiGestureEvent.h"

#include "imgui/imgui.h"

using namespace editor;

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
    io.KeyMap[ImGuiKey_Tab] = Key::TAB;
    //io.KeyMap[ImGuiKey_LeftArrow] = Key::LEFT;
    //io.KeyMap[ImGuiKey_RightArrow] = Key::RIGHT;
    //io.KeyMap[ImGuiKey_UpArrow] = Key::UP;
    //io.KeyMap[ImGuiKey_DownArrow] = Key::DOWN;
    //io.KeyMap[ImGuiKey_PageUp] = SDL_SCANCODE_PAGEUP;
    //io.KeyMap[ImGuiKey_PageDown] = SDL_SCANCODE_PAGEDOWN;
    //io.KeyMap[ImGuiKey_Home] = SDL_SCANCODE_HOME;
    //io.KeyMap[ImGuiKey_End] = SDL_SCANCODE_END;
    io.KeyMap[ImGuiKey_Delete] = Key::DELETE;
    io.KeyMap[ImGuiKey_Backspace] = Key::BACKSPACE;
    io.KeyMap[ImGuiKey_Enter] = Key::ENTER;
    //io.KeyMap[ImGuiKey_Escape] = SDLK_ESCAPE;
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

bool ImGuiInputHandler::OnKeyDown(const event::KeyDownEvent& event)
{
    if(event.key >= 512)
        return false;

    ImGui::GetIO().KeysDown[event.key] = true;
    return ImGui::GetIO().WantCaptureKeyboard;
}

bool ImGuiInputHandler::OnKeyUp(const event::KeyUpEvent& event)
{
    if(event.key >= 512)
        return false;

    ImGui::GetIO().KeysDown[event.key] = false;
    return ImGui::GetIO().WantCaptureKeyboard;
}

bool ImGuiInputHandler::OnTextInput(const event::TextInputEvent& event)
{
    ImGui::GetIO().AddInputCharactersUTF8(event.text);
    return ImGui::GetIO().WantTextInput;
}

bool ImGuiInputHandler::OnMouseDown(const event::MouseDownEvent& event)
{
    if(event.key != MouseButton::LEFT && event.key != MouseButton::RIGHT)
        return false;

    if(event.key == MouseButton::LEFT)
        ImGui::GetIO().MouseDown[0] = true;
    else if(event.key == MouseButton::RIGHT)
        ImGui::GetIO().MouseDown[1] = true;

    return ImGui::GetIO().WantCaptureMouse;
}

bool ImGuiInputHandler::OnMouseUp(const event::MouseUpEvent& event)
{
    if(event.key != MouseButton::LEFT && event.key != MouseButton::RIGHT)
        return false;

    if(event.key == MouseButton::LEFT)
        ImGui::GetIO().MouseDown[0] = false;
    else if(event.key == MouseButton::RIGHT)
        ImGui::GetIO().MouseDown[1] = false;

    return ImGui::GetIO().WantCaptureMouse;
}

bool ImGuiInputHandler::OnMouseMove(const event::MouseMotionEvent& event)
{
    ImGui::GetIO().MousePos = ImVec2(event.screenX, event.screenY);
    return false;
}

bool ImGuiInputHandler::OnMouseWheel(const event::MouseWheelEvent& event)
{
    ImGui::GetIO().MouseWheel = event.y;
    return ImGui::GetIO().WantCaptureMouse;
}

bool ImGuiInputHandler::OnMultiGesture(const event::MultiGestureEvent& event)
{
    return ImGui::GetIO().WantCaptureMouse;
}


