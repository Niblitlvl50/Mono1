
#pragma once

#include <cstdint>
#include <cstring>
#include <algorithm>

namespace mono
{
    template <typename T, uint32_t SIZE>
    class CircularVector
    {
    public:

        CircularVector()
            : m_data(nullptr)
            , m_offset_index(0)
            , m_write_index(0)
            , m_length(0)
        {
            m_data = new T[SIZE];
            std::memset(m_data, 0, SIZE * sizeof(float));
        }

        ~CircularVector()
        {
            delete[] m_data;
        }

        void Push(const T& value)
        {
            if(IsFull())
                m_offset_index++;

            if(m_offset_index == SIZE)
                m_offset_index = 0;

            m_data[m_write_index] = value;
            m_write_index++;
            if(m_write_index == SIZE)
                m_write_index = 0;

            m_length = std::clamp(m_length + 1, 0u, SIZE);
        }

        const T& operator[](int index) const
        {
            const int real_index = (index + m_offset_index % m_length);
            return m_data[real_index];
        }

        uint32_t Offset() const
        {
            return m_offset_index;
        }

        uint32_t Length() const
        {
            return m_length;
        }

        uint32_t MaxSize() const
        {
            return SIZE;
        }

        bool IsFull() const
        {
            return m_length == SIZE;
        }

        const T* Data() const
        {
            return m_data;
        }

        template <typename F>
        void ForEach(F&& callback)
        {
            for(uint32_t index = m_offset_index; index < m_length; ++index)
                callback(m_data[index]);

            if(IsFull())
            {
                for(uint32_t index = 0; index < m_offset_index; ++index)
                    callback(m_data[index]);
            }
        }

    private:

        T* m_data;
        uint32_t m_offset_index;
        uint32_t m_write_index;
        uint32_t m_length;
    };
}
