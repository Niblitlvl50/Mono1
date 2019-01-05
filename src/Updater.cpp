
#include "Updater.h"
#include "IUpdatable.h"

using namespace mono;

void Updater::AddUpdatable(IUpdatable* updatable)
{
    m_updatables.push_back(updatable);
}

void Updater::Update(unsigned int milliseconds)
{
    for(IUpdatable* updatable : m_updatables)
        updatable->doUpdate(milliseconds);

    m_updatables.clear();
}