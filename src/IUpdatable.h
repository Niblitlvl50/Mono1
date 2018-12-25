
#pragma once

namespace mono
{
    class IUpdatable
    {
    public:
    
        virtual ~IUpdatable()
        { }

        //! Update function
        //! @param[in] delta Delta value of how many milliseconds since last update.
        virtual void doUpdate(unsigned int delta) = 0;
    };
}
