
#include "Camera.h"
#include "Math/Vector.h"
#include "Math/MathFunctions.h"

#include <cmath>

using namespace mono;

Camera::Camera()
    : m_viewport(0.0f, 0.0f, 640, 480),
      m_target_viewport(m_viewport)
{ }

void Camera::doUpdate(const UpdateContext& update_context)
{
    const float change = (m_target_viewport.mB.x - m_viewport.mB.x);
    const float xzero = std::abs(change);

    if(xzero != 0.0f)
    {
        const float aspect = m_viewport.mB.x / m_viewport.mB.y;
        math::ResizeQuad(m_viewport, change * 0.1f, aspect);
    }
}

void Camera::SetViewport(const math::Quad& viewport)
{
    m_viewport = viewport;
    m_target_viewport = viewport;
}

void Camera::SetTargetViewport(const math::Quad& target)
{
    m_target_viewport = target;
}

math::Quad Camera::GetViewport() const
{
    return m_viewport;
}

void Camera::SetPosition(const math::Vector& position)
{
    // The position is the middle of the quad, not the lower left corner.
    const math::Vector& xy = position - (m_viewport.mB * 0.5f);
    m_viewport.mA = xy;
}

math::Vector Camera::GetPosition() const
{
    return m_viewport.mA + (m_viewport.mB * 0.5f);
}

void Camera::SetWindowSize(const math::Vector& window_size)
{
    m_window_size = window_size;
}

const math::Vector& Camera::GetWindowSize() const
{
    return m_window_size;
}

math::Vector Camera::ScreenToWorld(const math::Vector& screen_pos) const
{
    const float ratio = m_window_size.x / m_window_size.y;

    math::Quad viewport = GetViewport();
    viewport.mB.y = viewport.mB.x / ratio;

    const math::Vector& scale = viewport.mB / m_window_size;
    
    const float screenX = screen_pos.x;
    const float screenY = m_window_size.y - screen_pos.y;
    
    const float tempx = screenX * scale.x;
    const float tempy = screenY * scale.y;
    
    return math::Vector(tempx + viewport.mA.x, tempy + viewport.mA.y);
}
