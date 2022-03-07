
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
    constexpr float camera_speed_zoom = 2.5f;
    constexpr float camera_speed_translation = 5.0f;

    const bool is_equal = math::IsPrettyMuchEquals(m_target_viewport_size.x, m_viewport_size.x);
    if(!is_equal)
    {
        const float delta = (m_target_viewport_size.x - m_viewport_size.x) * camera_speed_zoom * update_context.delta_s;
        const float aspect = m_viewport_size.x / m_viewport_size.y;

        m_viewport_size.x += delta * aspect;
        m_viewport_size.y += delta;
    }

    const bool is_position_equal = math::IsPrettyMuchEquals(m_target_position, m_position);
    if(!is_position_equal)
    {
        const math::Vector delta = (m_target_position - m_position) * camera_speed_translation * update_context.delta_s;
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

math::Vector Camera::GetTargetViewportSize() const
{
    return m_target_viewport_size;
}

math::Quad Camera::GetViewport() const
{
    const math::Vector half_viewport_size = (m_viewport_size / 2.0f);

    math::Quad viewport;
    viewport.bottom_left = m_position - half_viewport_size;
    viewport.top_right = m_position + half_viewport_size;

    return viewport + m_position_offset;
}

void Camera::Move(const math::Vector& delta)
{
    m_target_position += delta;
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

void Camera::SetPositionOffset(const math::Vector& offset)
{
    m_position_offset = offset;
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
    const float window_ratio = m_window_size.x / m_window_size.y;

    const math::Quad viewport = GetViewport();
    const math::Vector center = math::Center(viewport);
    const float half_new_height = math::Width(viewport) / window_ratio / 2.0f;

    math::Quad new_viewport = viewport;
    new_viewport.top_right.y = center.y + half_new_height;
    new_viewport.bottom_left.y = center.y - half_new_height;

    const math::Vector viewport_size = math::Size(new_viewport);
    const math::Vector scale = viewport_size / m_window_size;

    const float screen_x = screen_pos.x;
    const float screen_y = m_window_size.y - screen_pos.y;

    return viewport.bottom_left + (math::Vector(screen_x, screen_y) * scale);
}
