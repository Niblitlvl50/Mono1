
#include "Updater.h"
#include "IUpdatable.h"

using namespace mono;

void Updater::AddUpdatable(const IUpdatablePtr& updatable)
{
    m_updatables.push_back(updatable);
}

void Updater::Update(unsigned int milliseconds)
{
    //mCamera->Update(milliseconds);
    
    for(auto& updatable : m_updatables)
        updatable->doUpdate(milliseconds);

    m_updatables.clear();
}
