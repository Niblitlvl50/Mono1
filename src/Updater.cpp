
#include "Updater.h"
#include "IUpdatable.h"

using namespace mono;

void Updater::AddUpdatable(IUpdatable* updatable)
{
    m_updatables.push_back(updatable);
}

void Updater::Update(const UpdateContext& update_context)
{
    for(IUpdatable* updatable : m_updatables)
        updatable->doUpdate(update_context);

    m_updatables.clear();
}
