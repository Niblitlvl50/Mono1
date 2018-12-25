
#pragma once

#include "MonoFwd.h"

namespace mono
{
    class IUpdater
    {
    public:

        virtual ~IUpdater()
        { }

        virtual void AddUpdatable(IUpdatable* updatable) = 0;
    };
}
