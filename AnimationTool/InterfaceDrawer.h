
#pragma once

#include "IUpdatable.h"

namespace animator
{
    struct UIContext;

    class InterfaceDrawer : public mono::IUpdatable
    {
    public:

        InterfaceDrawer(UIContext& context);
        virtual void doUpdate(unsigned int delta);

    private:

        UIContext& m_context;
    };
}
