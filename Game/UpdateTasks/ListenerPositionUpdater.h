
#pragma once

#include "MonoPtrFwd.h"
#include "IUpdatable.h"

namespace game
{
    class ListenerPositionUpdater : public mono::IUpdatable
    {
    public:
        ListenerPositionUpdater(const mono::IEntityPtr& entity);
        virtual void doUpdate(unsigned int delta);

    private:
        const mono::IEntityPtr m_entity;
    };
}
