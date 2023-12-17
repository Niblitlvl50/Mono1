
#pragma once

#include "System/System.h"
#include <cstdio>

namespace mono
{
    class ScopedTimer
    {
    public:

        ScopedTimer(const char* scope_context)
            : m_start_time(System::GetPerformanceCounter())
            , m_scope_context(scope_context)
        { }

        ~ScopedTimer()
        {
            const float delta_ms = float(System::GetPerformanceCounter() - m_start_time) * 1000.0f / System::GetPerformanceFrequency();
            System::Log("%s: %f ms", m_scope_context, delta_ms);
        }

        const uint64_t m_start_time;
        const char* m_scope_context;
    };

    template <typename T>
    class ScopedTimerCallback
    {
    public:

        ScopedTimerCallback(T&& callback)
            : m_start_time(System::GetPerformanceCounter())
            , m_callback(callback)
        { }

        ~ScopedTimerCallback()
        {
            const float delta_ms = float(System::GetPerformanceCounter() - m_start_time) / System::GetPerformanceFrequency();
            m_callback(delta_ms);
        }

        const uint64_t m_start_time;
        T m_callback;
    };
}

#define CONCAT(x, y) x##y
#define UNIQUE_NAME(prefix, line) CONCAT(prefix, line)

#define SCOPED_TIMER_AUTO() \
    const mono::ScopedTimer UNIQUE_NAME(scoped_timer, __LINE__)(__PRETTY_FUNCTION__);

