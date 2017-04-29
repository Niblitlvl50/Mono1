
#include "CameraTool.h"

#include "Camera/ICamera.h"
#include "Math/Quad.h"
#include "Math/MathFunctions.h"

#include <cmath>

using namespace editor;

CameraTool::CameraTool(const mono::ICameraPtr& camera, const math::Vector& window_size)
    : m_camera(camera),
      m_windowSize(window_size),
      m_translate(false)
{ }

bool CameraTool::IsActive() const
{
    return m_translate;
}

void CameraTool::HandleMouseDown(const math::Vector& screen_position)
{
    m_translate = true;
    m_translateDelta = screen_position;
}

void CameraTool::HandleMouseUp(const math::Vector& screen_position)
{
    m_translate = false;
}

void CameraTool::HandleMousePosition(const math::Vector& screen_position)
{
    if(!m_translate)
        return;

    const math::Quad& viewport = m_camera->GetViewport();
    const math::Vector& scale = viewport.mB / m_windowSize;

    math::Vector delta = (screen_position - m_translateDelta);
    delta.y = -delta.y;
    delta *= scale;

    const math::Vector& cam_pos = m_camera->GetPosition();
    const math::Vector& new_pos = cam_pos - delta;

    m_camera->SetPosition(new_pos);

    m_translateDelta = screen_position;
}

void CameraTool::HandleMouseWheel(float x, float y)
{
    math::Quad quad = m_camera->GetViewport();

    const float multiplier = (y < 0.0f) ? 1.0f : -1.0f;
    const float resizeValue = quad.mB.x * 0.15f * multiplier;
    const float aspect = quad.mB.x / quad.mB.y;
    math::ResizeQuad(quad, resizeValue, aspect);

    m_camera->SetTargetViewport(quad);
}

void CameraTool::HandleMultiGesture(const math::Vector& screen_position, float distance)
{
    if(std::fabs(distance) < 1e-3)
        return;

    const float multiplier = (distance < 0.0f) ? -1.0f : 1.0f;
    HandleMouseWheel(0.0f, multiplier);
}
