
#pragma once

#include "IUpdater.h"
#include <vector>

namespace mono
{
    class Updater : public IUpdater
    {
    public:

        void AddUpdatable(const IUpdatablePtr& updatable) override;
        void Update(unsigned int milliseconds);

    private:

        std::vector<IUpdatablePtr> m_updatables;
    };
}
