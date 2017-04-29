
#pragma once

#include "IRenderBuffer.h"
#include "BufferFactory.h"

namespace mono
{
    class RenderBuffer : public IRenderBuffer
    {
    public:
    
        RenderBuffer(BufferType type, BufferData data, size_t count);
        ~RenderBuffer();

        void UpdateData(const void* data, size_t offset, size_t count);
        void Use() const;
        size_t Id() const;

    private:

        BufferType m_type;
        BufferData m_data;
        unsigned int m_buffer_id;
    };
}
