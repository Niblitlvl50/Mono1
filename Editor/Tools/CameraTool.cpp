//
//  CameraTool.cpp
//  MonoiOS
//
//  Created by Niklas Damberg on 03/07/16.
//
//

#include "CameraTool.h"

#include "ICamera.h"
#include "Quad.h"
#include "MathFunctions.h"
#include "SysKeycodes.h"

using namespace editor;

CameraTool::CameraTool(const mono::ICameraPtr& camera)
    : m_camera(camera)
{ }

void CameraTool::Start()
{ }

void CameraTool::End()
{ }

bool CameraTool::IsActive() const
{
    return m_translate;
}

void CameraTool::HandleMouseDown(const math::Vector2f& world_pos)
{
    m_translate = true;
    m_startPosition = world_pos;
    m_cameraStartPosition = m_camera->GetPosition();


    //m_translateDelta.x = event.screenX;
    //m_translateDelta.y = m_windowSize.y - event.screenY;
}

void CameraTool::HandleMouseUp(const math::Vector2f& world_pos)
{
    m_translate = false;
}

void CameraTool::HandleMousePosition(const math::Vector2f& world_pos)
{
    if(!m_translate)
        return;

    //const math::Quad& viewport = m_camera->GetViewport();
    //const math::Vector2f& scale = viewport.mB / m_windowSize;

    const math::Vector2f& delta = world_pos - m_startPosition;
    const math::Vector2f& new_pos = m_cameraStartPosition - delta;

    //const math::Vector2f move(event.screenX, m_windowSize.y - event.screenY);
    //const math::Vector2f& delta = (move - m_translateDelta) * scale;

    //const math::Vector2f& cam_pos = m_camera->GetPosition();
    //const math::Vector2f& new_pos = cam_pos - delta;

    m_camera->SetPosition(new_pos);
    //m_translateDelta = move;
}

void CameraTool::HandleMultiGesture()
{
    /*
    if(std::fabs(event.distance) < 1e-3)
        return false;

    math::Quad quad = m_camera->GetViewport();

    const float multiplier = (event.distance < 0.0f) ? 1.0f : -1.0f;
    const float resizeValue = quad.mB.x * 0.15f * multiplier;
    const float aspect = quad.mB.x / quad.mB.y;
    math::ResizeQuad(quad, resizeValue, aspect);

    m_camera->SetTargetViewport(quad);
*/
}
