
#pragma once

#include "MonoPtrFwd.h"

namespace mono
{
    class IUpdater
    {
    public:

        virtual ~IUpdater()
        { }

        virtual void AddUpdatable(const IUpdatablePtr& updatable) = 0;
    };
}
