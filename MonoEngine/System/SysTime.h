//
//  Time.h
//  Monolith1
//
//  Created by Niblit on 2011-12-12.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//


#pragma once

#include <memory>
#include <functional>

namespace Time
{
    class ITimer
    {
    public:

        virtual ~ITimer()
        { }

        virtual void Start() = 0;
        virtual void Stop() = 0;
    };

    using ITimerPtr = std::unique_ptr<ITimer>;

    //! Create a timer with a single callback after a certain time
    ITimerPtr CreateOneShotTimer(unsigned int ms, const std::function<void ()>& callback);

    //! Creates a timer with repeating callbacks at a certain intervall
    ITimerPtr CreateRepeatingTimer(unsigned int ms, const std::function<void ()>& callback);

    //! Gets the system time in milliseconds
    unsigned int GetMilliseconds();

    //! Sleep for some milliseconds
    void Sleep(unsigned int milliseconds);
}

