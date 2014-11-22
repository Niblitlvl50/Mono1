/*
 *  EventToken.h
 *  Monolith1
 *
 *  Created by Niblit on 2011-02-16.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */


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

        bool operator < (const EventToken<Event>& other) const
        {
            return mId < other.mId;
        }

    private:
        unsigned int mId;
    };
}

