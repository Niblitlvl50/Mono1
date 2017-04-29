
#include "ListenerPositionUpdater.h"
#include "Audio/AudioListener.h"
#include "Math/Vector.h"
#include "Entity/IEntity.h"

using namespace game;

ListenerPositionUpdater::ListenerPositionUpdater(const mono::IEntityPtr& entity)
    : m_entity(entity)
{ }

void ListenerPositionUpdater::doUpdate(unsigned int delta)
{
    const math::Vector& position = m_entity->Position();
    mono::ListenerPosition(position.x, position.y);
}
