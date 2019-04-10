
#pragma once

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
        virtual void Update(uint32_t delta_ms) = 0;
        virtual void Sync() { }
    };
}
