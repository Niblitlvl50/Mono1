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
#include <unordered_map>
#include <functional>
#include <memory>
#include "EventToken.h"


namespace mono
{
    template<typename Event>
    struct EventListeners
    {
        typedef std::function<bool (const Event& event)> ListenerCallback;
        //std::unordered_map<EventToken<Event>, ListenerCallback> mListeners;
        std::map<EventToken<Event>, ListenerCallback> mListeners;


        inline EventToken<Event> AddListener(const ListenerCallback& listener)
        {
            EventToken<Event> token;
            mListeners.insert(std::make_pair(token, listener));
            return token;
        }

        inline void RemoveListener(const EventToken<Event>& token)
        {
            auto it = mListeners.find(token);
            if(it != mListeners.end())
                mListeners.erase(it);
        }

        inline void DispatchEvent(const Event& event)
        {
            for(auto& listener : mListeners)
            {
                const bool handled = listener.second(event);
                if(handled)
                    break;
            }
        }
    };    
    
    class EventHandler
    {
        typedef std::shared_ptr<void> voidPtr;
        std::unordered_map<const char*, voidPtr> events;

    public:
        
        template<typename Event>
        inline EventToken<Event> AddListener(const std::function<bool (const Event& event)>& listener)
        {
            const char* eventName = typeid(Event).name();
            auto it = events.find(eventName);
            if(it == events.end())
            {
                auto listeners = std::make_shared<EventListeners<Event>>();
                it = events.insert(std::make_pair(eventName, listeners)).first;
            }
            
            return std::static_pointer_cast<EventListeners<Event>>(it->second)->AddListener(listener);
        }
        
        template<typename Event>
        inline void RemoveListener(const EventToken<Event>& token)
        {
            const char* eventName = typeid(Event).name();
            const auto it = events.find(eventName);
            if(it != events.end())
            {
                auto listener = std::static_pointer_cast<EventListeners<Event>>(it->second);
                listener->RemoveListener(token);
            }
        }
        
        template<typename Event>
        inline void DispatchEvent(const Event& event)
        {
            const char* eventName = typeid(Event).name();
            const auto it = events.find(eventName);
            if(it != events.end())
            {
                auto listener = std::static_pointer_cast<EventListeners<Event>>(it->second);
                listener->DispatchEvent(event);
            }	
        }
    };
}

