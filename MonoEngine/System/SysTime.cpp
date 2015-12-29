//
//  Time.cpp
//  Monolith1
//
//  Created by Niblit on 2011-12-12.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#include "SysTime.h"
#include "SysEvents.h"
#include "SDL_timer.h"
#include "SDL_events.h"

namespace
{
    Uint32 CallbackFunc(Uint32 interval, void* data);

    class Timer : public Time::ITimer
    {
    public:

        Timer(unsigned int ms, bool repeatingTimer, const std::function<void ()>& callback)
            : m_ms(ms),
              m_repeatingTimer(repeatingTimer),
              m_callback(callback)
        {
            Start();
        }

        ~Timer()
        {
            Stop();
        }

        virtual void Start()
        {
            if(m_started)
                return;

            m_started = true;
            m_timerId = SDL_AddTimer(m_ms, CallbackFunc, this);
        }

        virtual void Stop()
        {
            if(m_started)
                SDL_RemoveTimer(m_timerId);
        }

        void DoCallback()
        {
            m_callback();
            if(!m_repeatingTimer)
                Stop();
        }

    private:

        const unsigned int m_ms;
        const bool m_repeatingTimer;
        const std::function<void ()> m_callback;

        SDL_TimerID m_timerId = -1;
        bool m_started = false;
    };

    void SynchronizedCallback(void* data)
    {
        Timer* object = static_cast<Timer*>(data);
        object->DoCallback();
    }

    Uint32 CallbackFunc(Uint32 interval, void* data)
    {
        SDL_UserEvent userEvent;
        userEvent.type = SDL_USEREVENT;
        userEvent.code = Events::TIMER_CALLBACK;
        userEvent.data1 = reinterpret_cast<void*>(&SynchronizedCallback);
        userEvent.data2 = data;

        SDL_Event event;
        event.type = SDL_USEREVENT;
        event.user = userEvent;

        SDL_PushEvent(&event);

        return interval;
    }
}


Time::ITimerPtr Time::CreateOneShotTimer(unsigned int ms, const std::function<void ()>& callback)
{
    return std::unique_ptr<ITimer>(new Timer(ms, false, callback));
}

Time::ITimerPtr Time::CreateRepeatingTimer(unsigned int ms, const std::function<void ()>& callback)
{
    return std::unique_ptr<ITimer>(new Timer(ms, true, callback));
}

unsigned int Time::GetMilliseconds()
{
    return SDL_GetTicks();
}

void Time::Sleep(unsigned int milliseconds)
{
    SDL_Delay(milliseconds);
}
