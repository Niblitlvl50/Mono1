
#pragma once

#include "IUpdatable.h"
#include <cstdint>

namespace mono
{
    class IGameSystem : public mono::IUpdatable
    {
    public:

        virtual ~IGameSystem() = default;

        virtual const char* Name() const = 0;

        // Once before a zone begins.
        virtual void Begin() { }

        // End of each frame.
        virtual void Sync() { }

        // Once after a zone is finished.
        virtual void Reset() { }

        // Before the destructor is called.
        virtual void Destroy() { }
    };
}
