
#pragma once

#include <algorithm>
#include <vector>
#include <numeric>

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

    template <typename T, typename F>
    inline T* find_if(std::vector<T>& collection, const F& find_func)
    {
        const auto it = std::find_if(collection.begin(), collection.end(), find_func);
        if(it != collection.end())
            return &*it;

        return nullptr;
    }

    template <typename T, typename F>
    inline const T* find_if(const std::vector<T>& collection, const F& find_func)
    {
        const auto it = std::find_if(collection.begin(), collection.end(), find_func);
        if(it != collection.end())
            return &*it;

        return nullptr;
    }

    template <typename T>
    inline uint32_t find_index_of(const std::vector<T>& collection, const T& value)
    {
        const auto it = std::find(collection.begin(), collection.end(), value);
        if(it == collection.end())
            return -1;

        return std::distance(collection.begin(), it);
    }

    template <typename T>
    inline void make_unique(std::vector<T>& collection)
    {
        std::sort(collection.begin(), collection.end());
        const auto new_end_it = std::unique(collection.begin(), collection.end());
        collection.erase(new_end_it, collection.end());
    }

    template<typename ForwardIt, typename T, typename Compare>
    inline std::pair<ForwardIt, ForwardIt> equal_range(ForwardIt first, ForwardIt last, const T& value, Compare comp)
    {
        return std::make_pair(
            std::lower_bound(first, last, value, comp),
            std::upper_bound(first, last, value, comp));
    }


    template <typename ForwardIt, typename Compare>
    inline std::vector<std::size_t> sort_permutation(ForwardIt begin, ForwardIt end, const Compare& compare)
    {
        std::vector<std::size_t> p(std::distance(begin, end));
        std::iota(p.begin(), p.end(), 0);

        std::sort(p.begin(), p.end(), [&](std::size_t i, std::size_t j) {
            return compare(*(begin + i), *(begin + j));
        });

        return p;
    }

    template <typename T>
    inline std::vector<T> apply_permutation(const std::vector<std::size_t>& p, const std::vector<T>& vec)
    {
        std::vector<T> sorted_vec(vec.size());
        std::transform(p.begin(), p.end(), sorted_vec.begin(), [&](std::size_t i) {
            return vec[i];
        });

        if(p.size() < vec.size())
        {
            const size_t offset = p.size();
            std::copy(vec.begin() + offset, vec.end(), sorted_vec.begin() + offset);
        }

        return sorted_vec;
    }

    template <typename T>
    inline void apply_permutation_in_place(const std::vector<std::size_t>& p, std::vector<T>& vec)
    {
        std::vector<bool> done(vec.size(), false);

        for (std::size_t i = 0; i < vec.size(); ++i)
        {
            if (done[i])
                continue;
            done[i] = true;

            std::size_t prev_j = i;
            std::size_t j = p[i];

            while (i != j)
            {
                std::swap(vec[prev_j], vec[j]);
                done[j] = true;
                prev_j = j;
                j = p[j];
            }
        }
    }

    template <typename T1, typename ... T>
    inline void apply_permutation_in_place(const std::vector<std::size_t>& p, std::vector<T1>& first, std::vector<T>&... args)
    {
        apply_permutation_in_place(p, first);
        apply_permutation_in_place(p, args...);
    }
}
