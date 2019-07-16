
#pragma once

#include "IUpdatable.h"
#include <cstdint>

struct ObjectAttribute;

namespace mono
{
    class IGameSystem
    {
    public:

        virtual ~IGameSystem() = default;
        virtual uint32_t Id() const = 0;
        virtual const char* Name() const = 0;
        virtual uint32_t Capacity() const = 0;
        virtual void Update(const mono::UpdateContext& update_context) = 0;
        virtual void Sync() { }
    };
}
