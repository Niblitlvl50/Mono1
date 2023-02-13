
#include "CameraController.h"
#include "ICamera.h"
#include "EventHandler/EventHandler.h"

#include "Events/EventFuncFwd.h"
#include "Events/MouseEvent.h"
#include "Events/MultiGestureEvent.h"

#include "Math/MathFunctions.h"
#include "Math/Quad.h"

#include <functional>
#include <cmath>

using namespace mono;

MouseCameraController::MouseCameraController(ICamera* camera, EventHandler* event_handler)
    : m_camera(camera),
      m_event_handler(event_handler),
      m_enabled(false),
      m_translate(false)
{
    using namespace std::placeholders;
    
    const event::MouseDownEventFunc mouse_down_func = std::bind(&MouseCameraController::OnMouseDown, this, _1);
    const event::MouseUpEventFunc mouse_up_func = std::bind(&MouseCameraController::OnMouseUp, this, _1);
    const event::MouseMotionEventFunc mouse_move_func = std::bind(&MouseCameraController::OnMouseMove, this, _1);
    const event::MultiGestureEventFunc multi_gesture_func = std::bind(&MouseCameraController::OnMultiGesture, this, _1);
    const event::MouseWheelEventFunc mouse_wheel_func = std::bind(&MouseCameraController::OnMouseWheel, this, _1);

    m_mouse_down_token = m_event_handler->AddListener(mouse_down_func);
    m_mouse_up_token = m_event_handler->AddListener(mouse_up_func);
    m_mouse_move_token = m_event_handler->AddListener(mouse_move_func);
    m_multi_gesture_token = m_event_handler->AddListener(multi_gesture_func);
    m_mouse_wheel_token = m_event_handler->AddListener(mouse_wheel_func);
}

MouseCameraController::~MouseCameraController()
{
    m_event_handler->RemoveListener(m_mouse_down_token);
    m_event_handler->RemoveListener(m_mouse_up_token);
    m_event_handler->RemoveListener(m_mouse_move_token);
    m_event_handler->RemoveListener(m_multi_gesture_token);
    m_event_handler->RemoveListener(m_mouse_wheel_token);
}

void MouseCameraController::Enable()
{
    m_enabled = true;
}

void MouseCameraController::Disable()
{
    m_enabled = false;
}

mono::EventResult MouseCameraController::OnMouseDown(const event::MouseDownEvent& event)
{
    if(m_enabled)
    {
        m_translate = true;
        m_translate_delta = math::Vector(event.screen_x, event.screen_y);
    }

    return mono::EventResult::PASS_ON;
}

mono::EventResult MouseCameraController::OnMouseUp(const event::MouseUpEvent& event)
{
    if(m_enabled)
        m_translate = false;
        
    return mono::EventResult::PASS_ON;
}

mono::EventResult MouseCameraController::OnMouseMove(const event::MouseMotionEvent& event)
{
    if(!m_enabled || !m_translate)
        return mono::EventResult::PASS_ON;

    const math::Vector screen_position(event.screen_x, event.screen_y);

    const math::Quad& viewport = m_camera->GetViewport();
    const math::Vector& window_size = m_camera->GetWindowSize();
    
    const math::Vector& scale = math::Size(viewport) / window_size;

    math::Vector delta = (screen_position - m_translate_delta);
    delta.y = -delta.y;
    delta *= scale;

    const math::Vector& cam_pos = m_camera->GetPosition();
    const math::Vector& new_pos = cam_pos - delta;

    m_camera->SetPosition(new_pos);

    m_translate_delta = screen_position;

    return mono::EventResult::HANDLED;
}

mono::EventResult MouseCameraController::OnMultiGesture(const event::MultiGestureEvent& event)
{
    if(!m_enabled)
        return mono::EventResult::PASS_ON;

    if(std::fabs(event.distance) < 1e-3)
        return mono::EventResult::PASS_ON;

    math::Vector viewport_size = m_camera->GetViewportSize();

    const float multiplier = (event.distance < 0.0f) ? 1.0f : -1.0f;
    const float resize_value = viewport_size.x * multiplier;
    const float aspect = viewport_size.x / viewport_size.y;

    viewport_size.x += resize_value * aspect;
    viewport_size.y += resize_value;

    m_camera->SetTargetViewportSize(viewport_size);

    return mono::EventResult::HANDLED;
}

mono::EventResult MouseCameraController::OnMouseWheel(const event::MouseWheelEvent& event)
{
    if(!m_enabled)
        return mono::EventResult::PASS_ON;

    math::Vector viewport_size = m_camera->GetViewportSize();

    const float multiplier = (event.y < 0.0f) ? 0.5f : -0.5f;
    const float resize_value = viewport_size.x * multiplier;
    const float aspect = viewport_size.x / viewport_size.y;

    viewport_size.x += resize_value * aspect;
    viewport_size.y += resize_value;

    m_camera->SetTargetViewportSize(viewport_size);

    return mono::EventResult::HANDLED;
}
