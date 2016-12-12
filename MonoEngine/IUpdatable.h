
#pragma once

namespace mono
{
    struct IUpdatable
    {
        virtual ~IUpdatable()
        { }

        //! Update function
        //! @param[in] delta Delta value of how many milliseconds since last update.
        virtual void doUpdate(unsigned int delta) = 0;
    };
}
