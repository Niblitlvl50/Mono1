
#pragma once

#include <cstdint>

namespace mono
{
    class IUpdatable
    {
    public:
    
        virtual ~IUpdatable()
        { }

        //! Update function
        //! @param[in] delta Delta value of how many milliseconds since last update.
        virtual void doUpdate(uint32_t delta_ms) = 0;
    };
}
