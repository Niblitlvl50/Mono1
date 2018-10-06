
#pragma once

static unsigned int next_token_id = 0;

namespace mono
{
    template <typename Event>
    class EventToken
    {
    public:
        EventToken()
            : m_id(++next_token_id)
        { }

        inline bool operator == (const EventToken<Event>& other) const
        {
            return m_id == other.m_id;
        }

    private:
        unsigned int m_id;
    };
}
