
#pragma once

#include <vector>
#include <cassert>
#include <cstdint>

namespace mono
{
    template <typename T>
    class ActiveVector
    {
    public:

        ActiveVector(uint32_t size)
        {
            m_types.resize(size);
            m_active.resize(size, false);
        }

        T* Set(uint32_t index, T&& data)
        {
            assert(m_active[index] == false);

            m_active[index] = true;
            m_types[index] = std::move(data);

            return &m_types[index];
        }

        T* Get(uint32_t index)
        {
            assert(m_active[index] == true);
            return &m_types[index];
        }

        void Release(uint32_t index)
        {
            m_active[index] = false;
        }

        bool IsActive(uint32_t index) const
        {
            return m_active[index];
        }

        template<typename CB>
        void ForEach(CB&& callable)
        {
            for(uint32_t index = 0; index < m_active.size(); ++index)
            {
                const bool is_active = m_active[index];
                if(is_active)
                    callable(index, m_types[index]);
            }
        }

        std::vector<T> m_types;
        std::vector<bool> m_active;
    };
}
