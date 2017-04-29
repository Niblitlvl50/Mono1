
#pragma once

static unsigned int currentId = 0;

namespace mono
{
    template <typename Event>
    class EventToken
    {
    public:
        EventToken()
            : mId(++currentId)
        { }

        inline bool operator < (const EventToken<Event>& other) const
        {
            return mId < other.mId;
        }

    private:
        unsigned int mId;
    };
}
