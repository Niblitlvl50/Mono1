//
//  EditorCameraController.cpp
//  MonoiOS
//
//  Created by Niklas Damberg on 26/06/16.
//
//

#include "EditorCameraController.h"

#include "EventHandler.h"
#include "EventFuncFwd.h"
#include "MouseDownEvent.h"
#include "MouseUpEvent.h"
#include "MouseMotionEvent.h"
#include "MultiGestureEvent.h"

#include "ICamera.h"
#include "Quad.h"
#include "MathFunctions.h"

#include "SysKeycodes.h"

using namespace editor;

CameraController::CameraController(const mono::ICameraPtr& camera, const math::Vector2f& window_size, mono::EventHandler& event_handler)
    : m_camera(camera),
      m_windowSize(window_size),
      m_eventHandler(event_handler),
      m_translate(false)
{
    using namespace std::placeholders;

    const event::MouseUpEventFunc& mouse_up = std::bind(&CameraController::OnMouseUp, this, _1);
    const event::MouseDownEventFunc& mouse_down = std::bind(&CameraController::OnMouseDown, this, _1);
    const event::MouseMotionEventFunc& mouse_move = std::bind(&CameraController::OnMouseMove, this, _1);
    const event::MultiGestureEventFunc& multi_gesture = std::bind(&CameraController::OnMultiGesture, this, _1);

    m_mouseUpToken = m_eventHandler.AddListener(mouse_up);
    m_mouseDownToken = m_eventHandler.AddListener(mouse_down);
    m_mouseMoveToken = m_eventHandler.AddListener(mouse_move);
    m_multiGestureToken = m_eventHandler.AddListener(multi_gesture);
}

CameraController::~CameraController()
{
    m_eventHandler.RemoveListener(m_mouseUpToken);
    m_eventHandler.RemoveListener(m_mouseDownToken);
    m_eventHandler.RemoveListener(m_mouseMoveToken);
    m_eventHandler.RemoveListener(m_multiGestureToken);
}

bool CameraController::OnMouseDown(const event::MouseDownEvent& event)
{
    if(event.key == MouseButton::LEFT)
    {
        m_translate = true;
        m_translateDelta.x = event.screenX;
        m_translateDelta.y = m_windowSize.y - event.screenY;
    }

    return false;
}

bool CameraController::OnMouseUp(const event::MouseUpEvent& event)
{
    m_translate = false;
    return false;
}

bool CameraController::OnMouseMove(const event::MouseMotionEvent& event)
{
    if(m_translate)
    {
        const math::Quad& viewport = m_camera->GetViewport();
        const math::Vector2f& scale = viewport.mB / m_windowSize;

        const math::Vector2f move(event.screenX, m_windowSize.y - event.screenY);
        const math::Vector2f& delta = (move - m_translateDelta) * scale;

        const math::Vector2f& cam_pos = m_camera->GetPosition();
        const math::Vector2f& new_pos = cam_pos - delta;

        m_camera->SetPosition(new_pos);

        m_translateDelta = move;
    }

    return false;
}

bool CameraController::OnMultiGesture(const event::MultiGestureEvent& event)
{
    if(std::fabs(event.distance) < 1e-3)
        return false;

    math::Quad quad = m_camera->GetViewport();

    const float multiplier = (event.distance < 0.0f) ? 1.0f : -1.0f;
    const float resizeValue = quad.mB.x * 0.15f * multiplier;
    const float aspect = quad.mB.x / quad.mB.y;
    math::ResizeQuad(quad, resizeValue, aspect);

    m_camera->SetTargetViewport(quad);

    return true;
}
