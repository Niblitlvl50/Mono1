
#pragma once

#include "System/System.h"
#include "MonoFwd.h"
#include <memory>

namespace game
{
    class Spawner
    {
    public:

        Spawner(mono::EventHandler& eventHandler);

    private:

        std::unique_ptr<System::ITimer> m_timer;
    };
}
