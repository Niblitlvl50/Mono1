
#include "InputHandler.h"
#include "EventHandler/EventHandler.h"

#include "Events/QuitEvent.h"
#include "Events/PauseEvent.h"
#include "Events/ApplicationEvent.h"
#include "Events/SurfaceChangedEvent.h"
#include "Events/ActivatedEvent.h"
#include "Events/KeyEvent.h"
#include "Events/TextInputEvent.h"
#include "Events/MouseEvent.h"
#include "Events/TouchEvent.h"
#include "Events/MultiGestureEvent.h"

#include "System/Keycodes.h"

using namespace mono;

InputHandler::InputHandler(const CoordinateFunc& func, EventHandler& eventHandler)
    : mScreenToWorldFunc(func),
      mEventHandler(eventHandler)
{ }

void InputHandler::OnKeyDown(Keycode key, bool ctrl, bool shift, bool alt)
{
    const event::KeyDownEvent event(key, ctrl, shift, alt);
    mEventHandler.DispatchEvent(event);
}

void InputHandler::OnKeyUp(Keycode key, bool ctrl, bool shift, bool alt)
{
    if(key == Keycode::ESCAPE)
    {
        mEventHandler.DispatchEvent(event::QuitEvent());
        return;
    }

    const event::KeyUpEvent event(key, ctrl, shift, alt);
    mEventHandler.DispatchEvent(event);
}

void InputHandler::OnTextInput(const char* text)
{
    const event::TextInputEvent event(text);
    mEventHandler.DispatchEvent(event);
}

void InputHandler::OnMouseDown(MouseButton button, int x, int y)
{
    float worldX = x;
    float worldY = y;
    mScreenToWorldFunc(worldX, worldY);
    const event::MouseDownEvent event(button, x, y, worldX, worldY);
    mEventHandler.DispatchEvent(event);
}

void InputHandler::OnMouseUp(MouseButton button, int x, int y)
{
    float worldX = x;
    float worldY = y;
    mScreenToWorldFunc(worldX, worldY);
    const event::MouseUpEvent event(button, x, y, worldX, worldY);
    mEventHandler.DispatchEvent(event);
}

void InputHandler::OnMouseMotion(int x, int y)
{
    float worldX = x;
    float worldY = y;
    mScreenToWorldFunc(worldX, worldY);
    const event::MouseMotionEvent event(x, y, worldX, worldY);
    mEventHandler.DispatchEvent(event);
}

void InputHandler::OnMouseWheel(int x, int y)
{
    const event::MouseWheelEvent event(x, y);
    mEventHandler.DispatchEvent(event);
}

void InputHandler::OnTouchDown(int64_t touchId, float x, float y, float dx, float dy)
{
    const event::TouchEvent event(event::TouchType::DOWN, touchId, x, y, dx, dy);
    mEventHandler.DispatchEvent(event);
}

void InputHandler::OnTouchUp(int64_t touchId, float x, float y, float dx, float dy)
{
    const event::TouchEvent event(event::TouchType::UP, touchId, x, y, dx, dy);
    mEventHandler.DispatchEvent(event);
}

void InputHandler::OnTouchMotion(int64_t touchId, float x, float y, float dx, float dy)
{
    const event::TouchEvent event(event::TouchType::MOTION, touchId, x, y, dx, dy);
    mEventHandler.DispatchEvent(event);
}

void InputHandler::OnMultiGesture(float x, float y, float theta, float distance)
{
    const event::MultiGestureEvent event(x, y, theta, distance);
    mEventHandler.DispatchEvent(event);
}

void InputHandler::OnUserEvent(int, void*, void*)
{ }

void InputHandler::OnAppTerminating()
{
    constexpr event::ApplicationEvent event(event::ApplicationState::TERMINATING);
    mEventHandler.DispatchEvent(event);
}

void InputHandler::OnEnterBackground()
{
    constexpr event::ApplicationEvent event(event::ApplicationState::ENTER_BACKGROUND);
    mEventHandler.DispatchEvent(event);
}

void InputHandler::OnEnterForeground()
{
    constexpr event::ApplicationEvent event(event::ApplicationState::ENTER_FOREGROUND);
    mEventHandler.DispatchEvent(event);
}

void InputHandler::OnQuit()
{
    constexpr event::QuitEvent event;
    mEventHandler.DispatchEvent(event);
}

void InputHandler::OnSurfaceChanged(int width, int height)
{
    const event::SurfaceChangedEvent event(width, height);
    mEventHandler.DispatchEvent(event);
}

void InputHandler::OnActivated(bool gain)
{
    const event::ActivatedEvent event(gain);
    mEventHandler.DispatchEvent(event);
}


