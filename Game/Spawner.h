
#pragma once

#include "System/SysTime.h"
#include "MonoFwd.h"

namespace game
{
    class Spawner
    {
    public:

        Spawner(mono::EventHandler& eventHandler);

    private:

        Time::ITimerPtr m_timer;
    };
}
