
#include "CameraController.h"
#include "EventHandler/EventHandler.h"

#include "Events/EventFuncFwd.h"
#include "Events/MouseEvent.h"
#include "Events/MultiGestureEvent.h"

#include "Rendering/ICamera.h"
#include "Math/MathFunctions.h"
#include "Math/Quad.h"

#include <functional>
#include <cmath>

using namespace mono;

CameraController::CameraController(int window_width, int window_height, ICamera* camera, EventHandler& event_handler)
    : m_window_width(window_width),
      m_window_height(window_height),
      m_camera(camera),
      m_event_handler(event_handler),
      m_enabled(true),
      m_translate(false)
{
    using namespace std::placeholders;
    
    const event::MouseDownEventFunc mouse_down_func = std::bind(&CameraController::OnMouseDown, this, _1);
    const event::MouseUpEventFunc mouse_up_func = std::bind(&CameraController::OnMouseUp, this, _1);
    const event::MouseMotionEventFunc mouse_move_func = std::bind(&CameraController::OnMouseMove, this, _1);
    const event::MultiGestureEventFunc multi_gesture_func = std::bind(&CameraController::OnMultiGesture, this, _1);

    m_mouse_down_token = m_event_handler.AddListener(mouse_down_func);
    m_mouse_up_token = m_event_handler.AddListener(mouse_up_func);
    m_mouse_move_token = m_event_handler.AddListener(mouse_move_func);
    m_multi_gesture_token = m_event_handler.AddListener(multi_gesture_func);
}

CameraController::~CameraController()
{
    m_event_handler.RemoveListener(m_mouse_down_token);
    m_event_handler.RemoveListener(m_mouse_up_token);
    m_event_handler.RemoveListener(m_mouse_move_token);
    m_event_handler.RemoveListener(m_multi_gesture_token);
}

bool CameraController::OnMouseDown(const event::MouseDownEvent& event)
{
    m_translate = true;
    m_translate_delta = math::Vector(event.screenX, event.screenY);
    return true;
}

bool CameraController::OnMouseUp(const event::MouseUpEvent& event)
{
    m_translate = false;
    return true;
}

bool CameraController::OnMouseMove(const event::MouseMotionEvent& event)
{
    if(!m_translate)
        return true;

    const math::Vector screen_position(event.screenX, event.screenY);

    const math::Quad& viewport = m_camera->GetViewport();
    const math::Vector window_size(m_window_width, m_window_height);
    
    const math::Vector& scale = viewport.mB / window_size;

    math::Vector delta = (screen_position - m_translate_delta);
    delta.y = -delta.y;
    delta *= scale;

    const math::Vector& cam_pos = m_camera->GetPosition();
    const math::Vector& new_pos = cam_pos - delta;

    m_camera->SetPosition(new_pos);

    m_translate_delta = screen_position;

    return true;
}

bool CameraController::OnMultiGesture(const event::MultiGestureEvent& event)
{
    if(!m_enabled)
        return false;

    if(std::fabs(event.distance) < 1e-3)
        return false;

    math::Quad quad = m_camera->GetViewport();

    const float multiplier = (event.distance < 0.0f) ? 1.0f : -1.0f;
    const float resize_value = quad.mB.x * 0.15f * multiplier;
    const float aspect = quad.mB.x / quad.mB.y;
    math::ResizeQuad(quad, resize_value, aspect);

    m_camera->SetTargetViewport(quad);

    return true;
}
