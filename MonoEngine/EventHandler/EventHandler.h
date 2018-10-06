
#pragma once

#include <typeinfo>
#include <unordered_map>
#include <functional>
#include <memory>
#include <vector>
#include "EventToken.h"

namespace mono
{
    template<typename Event>
    struct EventListeners
    {
        using ListenerCallback = std::function<bool (const Event&)>;

        std::vector<EventToken<Event>> m_tokens;
        std::vector<ListenerCallback> m_callbacks;

        inline EventToken<Event> AddListener(const ListenerCallback& callback)
        {
            m_tokens.push_back(EventToken<Event>());
            m_callbacks.push_back(callback);
            return m_tokens.back();
        }

        inline void RemoveListener(const EventToken<Event>& token)
        {
            auto it = std::find(m_tokens.begin(), m_tokens.end(), token);
            if(it != m_tokens.end())
            {
                const size_t offset = std::distance(m_tokens.begin(), it);

                m_tokens.erase(it);
                m_callbacks.erase(m_callbacks.begin() + offset);
            }
        }

        inline void DispatchEvent(const Event& event)
        {
            for(const ListenerCallback& callback : m_callbacks)
            {
                const bool handled = callback(event);
                if(handled)
                    break;
            }
        }
    };    
    
    class EventHandler
    {
        std::unordered_map<size_t, std::shared_ptr<void>> m_events;

    public:
        
        template<typename Event>
        inline EventToken<Event> AddListener(const std::function<bool (const Event& event)>& listener)
        {
            const size_t event_hash = typeid(Event).hash_code();
            auto it = m_events.find(event_hash);
            if(it == m_events.end())
            {
                auto listeners = std::make_shared<EventListeners<Event>>();
                it = m_events.insert(std::make_pair(event_hash, listeners)).first;
            }
            
            return std::static_pointer_cast<EventListeners<Event>>(it->second)->AddListener(listener);
        }
        
        template<typename Event>
        inline void RemoveListener(const EventToken<Event>& token)
        {
            const size_t event_hash = typeid(Event).hash_code();
            const auto it = m_events.find(event_hash);
            if(it != m_events.end())
            {
                auto listener = std::static_pointer_cast<EventListeners<Event>>(it->second);
                listener->RemoveListener(token);
            }
        }
        
        template<typename Event>
        inline void DispatchEvent(const Event& event)
        {
            const size_t event_hash = typeid(Event).hash_code();
            const auto it = m_events.find(event_hash);
            if(it != m_events.end())
            {
                auto listener = std::static_pointer_cast<EventListeners<Event>>(it->second);
                listener->DispatchEvent(event);
            }	
        }
    };
}

