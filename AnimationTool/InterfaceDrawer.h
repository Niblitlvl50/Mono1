
#pragma once

#include "IUpdatable.h"

namespace animator
{
    class InterfaceDrawer : public mono::IUpdatable
    {
    public:
        virtual void doUpdate(unsigned int delta);
    };
}
