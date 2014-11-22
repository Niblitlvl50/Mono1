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
        typedef std::function<void (const Event& event)> ListenerCallback;
        std::map<EventToken<Event>, ListenerCallback> mListeners;

        EventToken<Event> AddListener(ListenerCallback listener)
        {
            EventToken<Event> token;
            mListeners.insert(std::make_pair(token, listener));
            return token;
        }

        void RemoveListener(EventToken<Event>& token)
        {
            auto it = mListeners.find(token);
            if(it != mListeners.end())
                mListeners.erase(it);
        }

        void DispatchEvent(const Event& event)
        {
            for(auto& listener : mListeners)
                listener.second(event);
        }
    };    
    
    class EventHandler
    {
        typedef std::shared_ptr<void> voidPtr;
        typedef std::map<std::string, voidPtr> EventCollection;
        EventCollection events;
        
    public:
        
        template<typename Event>
        EventToken<Event> AddListener(std::function<void (const Event& event)> listener)
        {
            const std::string eventName = typeid(Event).name();
            auto it = events.find(eventName);
            if(it == events.end())
            {
                auto listeners = std::make_shared<EventListeners<Event>>();
                it = events.insert(std::make_pair(eventName, listeners)).first;
            }
            
            return std::static_pointer_cast<EventListeners<Event>>(it->second)->AddListener(listener);
        }
        
        template<class Event>
        void RemoveListener(EventToken<Event>& token)
        {
            const std::string eventName = typeid(Event).name();
            const auto it = events.find(eventName);
            if(it != events.end())
            {
                auto listener = std::static_pointer_cast<EventListeners<Event>>(it->second);
                listener->RemoveListener(token);
            }
        }
        
        template<typename Event>
        void DispatchEvent(const Event& event)
        {
            const std::string eventName = typeid(Event).name();
            const auto it = events.find(eventName);
            if(it != events.end())
            {
                auto listener = std::static_pointer_cast<EventListeners<Event>>(it->second);
                listener->DispatchEvent(event);
            }	
        }
    };
}

