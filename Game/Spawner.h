//
//  Spawner.hpp
//  MonoOSX
//
//  Created by Niklas Damberg on 03/10/15.
//
//

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
