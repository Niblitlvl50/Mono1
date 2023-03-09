
#pragma once

#include <vector>
#include <algorithm>
#include <numeric>
#include <cstdint>

namespace mono
{
    template <typename T>
    class ObjectPool
    {
    public:
    
        ObjectPool(size_t pool_size)
        {
            m_data.resize(pool_size);
            m_free_indices.resize(pool_size);
            
            std::iota(m_free_indices.begin(), m_free_indices.end(), 0);
            std::reverse(m_free_indices.begin(), m_free_indices.end());
        }

        T* GetPoolData(uint32_t* out_index = nullptr)
        {
            if(m_free_indices.empty())
                return nullptr;

            const uint32_t free_index = m_free_indices.back();
            m_free_indices.pop_back();
            if(out_index)
                *out_index = free_index;

            return GetPoolDataByIndex(free_index);
        }

        void ReleasePoolData(const T* data)
        {
            for(uint32_t index = 0; index < m_data.size(); ++index)
            {
                T* other_data = &m_data[index];
                if(data == other_data)
                {
                    ReleasePoolData(index);
                    break;
                }
            }
        }

        void ReleasePoolData(uint32_t index)
        {
            m_free_indices.push_back(index);
        }

        T* GetPoolDataByIndex(uint32_t index)
        {
            return &m_data[index];
        }

        size_t Used() const
        {
            return Size() - m_free_indices.size();
        }

        size_t Size() const
        {
            return m_data.size();
        }

        T* Data()
        {
            return m_data.data();
        }

    private:

        std::vector<T> m_data;
        std::vector<uint32_t> m_free_indices;
    };
}
