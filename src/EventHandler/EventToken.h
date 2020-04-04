
#pragma once

namespace mono
{
    enum class EventResult
    {
        HANDLED,
        PASS_ON,
    };

    template <typename Event>
    struct EventToken
    {
        inline bool operator == (const EventToken& other) const
        {
            return m_id == other.m_id;
        }

        unsigned int m_id;
    };
}
