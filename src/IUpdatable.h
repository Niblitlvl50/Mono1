
#pragma once

#include <cstdint>

namespace mono
{
    struct UpdateContext
    {
        uint32_t frame_count;
        uint32_t delta_ms;
        uint32_t timestamp;
    };

    class IUpdatable
    {
    public:
    
        virtual ~IUpdatable() = default;
        virtual void Update(const mono::UpdateContext& update_context) = 0;
    };
}
