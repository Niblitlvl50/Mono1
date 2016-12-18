
#pragma once

#include "MonoPtrFwd.h"
#include "IUpdatable.h"

namespace game
{
    class ListenerPositionUpdater : public mono::IUpdatable
    {
    public:

        ListenerPositionUpdater(const mono::ICameraPtr& camera);
        virtual void doUpdate(unsigned int delta);

    private:

        mono::ICameraPtr m_camera;
    };
}
