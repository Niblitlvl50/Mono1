//
//  Spawner.hpp
//  MonoOSX
//
//  Created by Niklas Damberg on 03/10/15.
//
//

#pragma once

#include "SysTime.h"
#include "MonoFwd.h"

namespace game
{
    class Spawner
    {
    public:

        Spawner(mono::EventHandler& eventHandler);

        void OnSpawnEntity();

    private:

        mono::EventHandler& m_eventHandler;
        Time::ITimerPtr m_timer;
    };
}
