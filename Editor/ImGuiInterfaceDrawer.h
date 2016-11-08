//
//  ImGuiInterfaceDrawer.hpp
//  MonoiOS
//
//  Created by Niklas Damberg on 20/06/16.
//
//

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

