
#include "InputHandler.h"
#include "EventHandler/EventHandler.h"

#include "Events/QuitEvent.h"
#include "Events/ApplicationEvent.h"
#include "Events/SurfaceChangedEvent.h"
#include "Events/ActivatedEvent.h"
#include "Events/KeyEvent.h"
#include "Events/TextInputEvent.h"
#include "Events/MouseEvent.h"
#include "Events/TouchEvent.h"
#include "Events/MultiGestureEvent.h"
#include "Events/ControllerEvent.h"

#include "System/Keycodes.h"

using namespace mono;

InputHandler::InputHandler(const ScreenToWorldFunc& screen_to_world_func, EventHandler* event_handler)
    : m_screen_to_world_func(screen_to_world_func)
    , m_event_handler(event_handler)
{ }

void InputHandler::OnKeyDown(Keycode key, bool ctrl, bool shift, bool alt, bool super, uint32_t timestamp)
{
    const event::KeyDownEvent event(key, ctrl, shift, alt, super, timestamp);
    m_event_handler->DispatchEvent(event);
}

void InputHandler::OnKeyUp(Keycode key, bool ctrl, bool shift, bool alt, bool super, uint32_t timestamp)
{
    if(key == Keycode::ESCAPE)
    {
        OnQuit();
        return;
    }

    const event::KeyUpEvent event(key, ctrl, shift, alt, super, timestamp);
    m_event_handler->DispatchEvent(event);
}

void InputHandler::OnTextInput(const char* text)
{
    const event::TextInputEvent event(text);
    m_event_handler->DispatchEvent(event);
}

void InputHandler::OnMouseDown(MouseButton button, int x, int y, bool ctrl, bool shift, bool alt, bool super, uint32_t timestamp)
{
    float world_x = x;
    float world_y = y;
    m_screen_to_world_func(world_x, world_y);
    const event::MouseDownEvent event(button, x, y, world_x, world_y, ctrl, shift, alt, super, timestamp);
    m_event_handler->DispatchEvent(event);
}

void InputHandler::OnMouseUp(MouseButton button, int x, int y, bool ctrl, bool shift, bool alt, bool super, uint32_t timestamp)
{
    float world_x = x;
    float world_y = y;
    m_screen_to_world_func(world_x, world_y);
    const event::MouseUpEvent event(button, x, y, world_x, world_y, ctrl, shift, alt, super, timestamp);
    m_event_handler->DispatchEvent(event);
}

void InputHandler::OnMouseMotion(int x, int y, bool ctrl, bool shift, bool alt, bool super, uint32_t timestamp)
{
    float world_x = x;
    float world_y = y;
    m_screen_to_world_func(world_x, world_y);
    const event::MouseMotionEvent event(x, y, world_x, world_y, ctrl, shift, alt, super, timestamp);
    m_event_handler->DispatchEvent(event);
}

void InputHandler::OnMouseWheel(int x, int y, bool ctrl, bool shift, bool alt, bool super, uint32_t timestamp)
{
    const event::MouseWheelEvent event(x, y, ctrl, shift, alt, super, timestamp);
    m_event_handler->DispatchEvent(event);
}

void InputHandler::OnTouchDown(int64_t touchId, float x, float y, float dx, float dy)
{
    const event::TouchEvent event(event::TouchType::DOWN, touchId, x, y, dx, dy);
    m_event_handler->DispatchEvent(event);
}

void InputHandler::OnTouchUp(int64_t touchId, float x, float y, float dx, float dy)
{
    const event::TouchEvent event(event::TouchType::UP, touchId, x, y, dx, dy);
    m_event_handler->DispatchEvent(event);
}

void InputHandler::OnTouchMotion(int64_t touchId, float x, float y, float dx, float dy)
{
    const event::TouchEvent event(event::TouchType::MOTION, touchId, x, y, dx, dy);
    m_event_handler->DispatchEvent(event);
}

void InputHandler::OnMultiGesture(float x, float y, float theta, float distance)
{
    const event::MultiGestureEvent event(x, y, theta, distance);
    m_event_handler->DispatchEvent(event);
}

void InputHandler::OnControllerAdded(int controller_id)
{
    const event::ControllerAddedEvent event(controller_id);
    m_event_handler->DispatchEvent(event);
}

void InputHandler::OnControllerRemoved(int controller_id)
{
    const event::ControllerRemovedEvent event(controller_id);
    m_event_handler->DispatchEvent(event);
}

void InputHandler::OnControllerButtonDown(int controller_id, System::ControllerButton button, uint32_t timestamp)
{
    const event::ControllerButtonDownEvent event(controller_id, button, timestamp);
    m_event_handler->DispatchEvent(event);
}

void InputHandler::OnAppTerminating()
{
    constexpr event::ApplicationEvent event(event::ApplicationState::TERMINATING);
    m_event_handler->DispatchEvent(event);
}

void InputHandler::OnEnterBackground()
{
    constexpr event::ApplicationEvent event(event::ApplicationState::ENTER_BACKGROUND);
    m_event_handler->DispatchEvent(event);
}

void InputHandler::OnEnterForeground()
{
    constexpr event::ApplicationEvent event(event::ApplicationState::ENTER_FOREGROUND);
    m_event_handler->DispatchEvent(event);
}

void InputHandler::OnQuit()
{
    constexpr event::QuitEvent event;
    m_event_handler->DispatchEvent(event);
}

void InputHandler::OnSurfaceChanged(int width, int height)
{
    const event::SurfaceChangedEvent event(width, height);
    m_event_handler->DispatchEvent(event);
}

void InputHandler::OnActivated(bool gain)
{
    const event::ActivatedEvent event(gain);
    m_event_handler->DispatchEvent(event);
}
