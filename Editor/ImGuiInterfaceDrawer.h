
#pragma once

#include "IUpdatable.h"

namespace editor
{
    struct UIContext;

    class ImGuiInterfaceDrawer : public mono::IUpdatable
    {
    public:
        ImGuiInterfaceDrawer(UIContext& context);
        virtual void doUpdate(unsigned int delta);
    
    private:
        UIContext& m_context;
    };
}

