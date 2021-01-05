
#include "Camera.h"
#include "Math/Vector.h"
#include "Math/MathFunctions.h"
#include "Math/EasingFunctions.h"

#include <cmath>

using namespace mono;

Camera::Camera()
    : m_viewport_size(640.0f, 480.0f)
    , m_target_viewport_size(m_viewport_size)
{ }

void Camera::Update(const UpdateContext& update_context)
{
    constexpr float camera_speed = 5.0f;

    const float delta_s = float(update_context.delta_ms) / 1000.0f;

    const bool is_equal = math::IsPrettyMuchEquals(m_target_viewport_size.x, m_viewport_size.x);
    if(!is_equal)
    {
        const float delta = (m_target_viewport_size.x - m_viewport_size.x) * camera_speed * delta_s;
        const float aspect = m_viewport_size.x / m_viewport_size.y;

        m_viewport_size.x += delta * aspect;
        m_viewport_size.y += delta;
    }

    const bool is_position_equal = math::IsPrettyMuchEquals(m_target_position, m_position);
    if(!is_position_equal)
    {
        const math::Vector delta = (m_target_position - m_position) * camera_speed * delta_s;
        m_position += delta;
    }
}

void Camera::SetViewportSize(const math::Vector& size)
{
    m_viewport_size = size;
    m_target_viewport_size = size;
}

void Camera::SetTargetViewportSize(const math::Vector& target_size)
{
    m_target_viewport_size = target_size;
}

math::Vector Camera::GetViewportSize() const
{
    return m_viewport_size;
}

math::Quad Camera::GetViewport() const
{
    math::Quad viewport;
    viewport.mA = m_position - (m_viewport_size / 2.0f);
    viewport.mB = m_viewport_size;

    return viewport;
}

void Camera::SetPosition(const math::Vector& position)
{
    m_position = position;
    m_target_position = position;
}

void Camera::SetTargetPosition(const math::Vector& position)
{
    m_target_position = position;
}

math::Vector Camera::GetPosition() const
{
    return m_position;
}

math::Vector Camera::GetTargetPosition() const
{
    return m_target_position;
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
