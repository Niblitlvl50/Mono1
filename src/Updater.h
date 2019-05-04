
#pragma once

#include "IUpdater.h"
#include "IUpdatable.h"
#include <vector>

namespace mono
{
    class Updater : public IUpdater
    {
    public:

        void AddUpdatable(IUpdatable* updatable) override;
        void Update(const UpdateContext& update_context);

    private:

        std::vector<IUpdatable*> m_updatables;
    };
}
