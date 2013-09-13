/*
 *  EventHandler.h
 *  Monolith1
 *
 *  Created by Niblit on 2011-02-09.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once

#include <typeinfo>
#include <map>
#include <string>
#include <functional>
#include <memory>
#include "EventToken.h"


namespace mono
{
    template<typename Event>
    struct EventListeners
    {
        typedef std::function< void (const Event& event) > ListenerCallback;
        typedef typename std::map< EventToken<Event>, ListenerCallback >::iterator Iterator;
        std::map<EventToken<Event>, ListenerCallback> mListeners;

        EventToken<Event> AddListener(ListenerCallback listener)
        {
            EventToken<Event> token;
            mListeners.insert(std::make_pair(token, listener));
            return token;
        }

        void RemoveListener(EventToken<Event>& token)
        {
            Iterator it = mListeners.find(token);
            if(it != mListeners.end())
                mListeners.erase(it);
        }

        void DispatchEvent(const Event& event)
        {
            for(Iterator it = mListeners.begin(), end = mListeners.end(); it != end; ++it)
                it->second(event);
        }
    };


    class EventHandler
    {
        typedef std::shared_ptr<void> voidPtr;
        typedef std::map<std::string, voidPtr> EventCollection;
        static EventCollection events;

    public:

        template<typename Event>
        static EventToken<Event> AddListener(std::function< void (const Event& event) > listener)
        {
            const char* name = typeid(Event).name();
            const std::string eventName(name);

            typedef std::shared_ptr< EventListeners<Event> > EventListenersPtr;

            EventCollection::const_iterator it = events.find(eventName);
            if(it == events.end())
            {
                EventListenersPtr listeners(new EventListeners<Event>);
                events.insert(std::make_pair(eventName, listeners));
            }

            it = events.find(eventName);
            return std::static_pointer_cast< EventListeners<Event> >(it->second)->AddListener(listener);
        }

        template<class Event>
        static void RemoveListener(EventToken<Event>& token)
        {
            const char* name = typeid(Event).name();
            const std::string eventName(name);

            EventCollection::iterator it = events.find(eventName);
            if(it != events.end())
            {
                typedef std::shared_ptr< EventListeners<Event> > EventListenersPtr;
                EventListenersPtr listener = std::static_pointer_cast< EventListeners<Event> >(it->second);
                listener->RemoveListener(token);
            }
        }

        template<typename Event>
        static void DispatchEvent(const Event& event)
        {
            const char* name = typeid(Event).name();
            const std::string eventName(name);

            EventCollection::iterator it = events.find(eventName);
            if(it != events.end())
            {
                typedef std::shared_ptr< EventListeners<Event> > EventListenersPtr;
                EventListenersPtr listener = std::static_pointer_cast< EventListeners<Event> >(it->second);
                listener->DispatchEvent(event);
            }	
        }

    };
}

