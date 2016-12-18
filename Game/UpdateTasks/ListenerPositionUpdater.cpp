
#include "ListenerPositionUpdater.h"
#include "Audio/AudioListener.h"
#include "Math/Vector.h"
#include "ICamera.h"

using namespace game;

ListenerPositionUpdater::ListenerPositionUpdater(const mono::ICameraPtr& camera)
    : m_camera(camera)
{ }

void ListenerPositionUpdater::doUpdate(unsigned int delta)
{
    mono::ListenerPosition(m_camera->GetPosition());
}
