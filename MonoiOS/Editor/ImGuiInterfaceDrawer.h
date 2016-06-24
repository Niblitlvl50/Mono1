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
    class ImGuiInterfaceDrawer : public mono::IUpdatable
    {
    public:

        virtual void doUpdate(unsigned int delta);
    };
}

