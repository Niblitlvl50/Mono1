
#pragma once

#include <cstdint>

namespace mono
{
    struct UpdateContext
    {
        uint32_t timestamp;
        uint32_t frame_count;
        uint32_t delta_ms;
        float delta_s;
        bool paused;
    };

    class IUpdatable
    {
    public:
    
        virtual ~IUpdatable() = default;
        virtual void Update(const mono::UpdateContext& update_context) = 0;
        virtual bool UpdateInPause() const { return false; }
    };
}
