
#pragma once

#include <algorithm>
#include <vector>

namespace mono
{
    template <typename T>
    inline bool remove(std::vector<T>& collection, const T& object)
    {
        const auto it = std::find(collection.begin(), collection.end(), object);
        if(it != collection.end())
        {
            collection.erase(it);
            return true;
        }

        return false;
    }

    template <typename T, typename F>
    inline bool remove_if(std::vector<T>& collection, const F& remove_if_func)
    {
        const auto it = std::remove_if(collection.begin(), collection.end(), remove_if_func);
        if(it != collection.end())
        {
            collection.erase(it, collection.end());
            return true;
        }
        
        return false;
    }

    template <typename T>
    inline bool contains(const std::vector<T>& collection, const T& thing)
    {
        const auto it = std::find(collection.begin(), collection.end(), thing);
        return it != collection.end();
    }

    template<class ForwardIt, class T, class Compare>
    std::pair<ForwardIt,ForwardIt> equal_range(ForwardIt first, ForwardIt last, const T& value, Compare comp)
    {
        return std::make_pair(
            std::lower_bound(first, last, value, comp),
            std::upper_bound(first, last, value, comp));
    }
}
