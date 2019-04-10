
#include "CameraSwitch.h"
#include "Math/Quad.h"

using namespace mono;

CameraSwitch::CameraSwitch(const std::vector<ICameraPtr>& cameras)
    : m_cameras(cameras),
      m_active_camera(cameras.front())
{ }

void CameraSwitch::SetActiveCamera(int index)
{
    m_active_camera = m_cameras[index];
}

void CameraSwitch::doUpdate(unsigned int delta)
{
    m_active_camera->doUpdate(delta);
}

void CameraSwitch::Follow(uint32_t entity_id, const math::Vector& offset)
{
    m_active_camera->Follow(entity_id, offset);
}

void CameraSwitch::Unfollow()
{
    m_active_camera->Unfollow();
}

math::Quad CameraSwitch::GetViewport() const
{
    return m_active_camera->GetViewport();
}

math::Vector CameraSwitch::GetPosition() const
{
    return m_active_camera->GetPosition();    
}

void CameraSwitch::SetViewport(const math::Quad& viewport)
{
    return m_active_camera->SetViewport(viewport);    
}

void CameraSwitch::SetTargetViewport(const math::Quad& target)
{
    return m_active_camera->SetTargetViewport(target);    
}

void CameraSwitch::SetPosition(const math::Vector& position)
{
    return m_active_camera->SetPosition(position);    
}

math::Vector CameraSwitch::ScreenToWorld(const math::Vector& screen_pos, const math::Vector& window_size) const
{
    return m_active_camera->ScreenToWorld(screen_pos, window_size);
}
