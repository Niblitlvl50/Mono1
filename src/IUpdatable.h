
#pragma once

#include <cstdint>

namespace mono
{
    struct UpdateContext
    {
        uint32_t frame_count;
        uint32_t delta_ms;
        uint32_t total_time;
    };

    class IUpdatable
    {
    public:
    
        virtual ~IUpdatable()
        { }

        virtual void doUpdate(const UpdateContext& update_context) = 0;
    };
}
