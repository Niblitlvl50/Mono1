/*
 *  EventToken.h
 *  Monolith1
 *
 *  Created by Niblit on 2011-02-16.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */


#ifndef MONO_EVENTTOKEN_H
#define MONO_EVENTTOKEN_H

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
		
		unsigned int GetId() const
		{
			return mId;
		}
		
		bool operator < (const EventToken<Event>& other) const
		{
			return mId < other.GetId();
		}
		
	private:
        unsigned int mId;
	};

}


#endif

