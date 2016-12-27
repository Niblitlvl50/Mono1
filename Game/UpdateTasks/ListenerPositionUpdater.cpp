
#include "ListenerPositionUpdater.h"
#include "Audio/AudioListener.h"
#include "IEntity.h"

using namespace game;

ListenerPositionUpdater::ListenerPositionUpdater(const mono::IEntityPtr& entity)
    : m_entity(entity)
{ }

void ListenerPositionUpdater::doUpdate(unsigned int delta)
{
    mono::ListenerPosition(m_entity->Position());
}
