
#pragma once

#include "IUpdater.h"
#include <vector>

namespace mono
{
    class Updater : public IUpdater
    {
    public:

        void AddUpdatable(IUpdatable* updatable) override;
        void Update(unsigned int milliseconds);

    private:

        std::vector<IUpdatable*> m_updatables;
    };
}
