
#pragma once

#include "IRenderBuffer.h"
#include "BufferFactory.h"

namespace mono
{
    class RenderBuffer : public IRenderBuffer
    {
    public:
    
        RenderBuffer(mono::BufferTarget target, mono::BufferType type, mono::BufferData data, size_t count);
        ~RenderBuffer();

        void UpdateData(const void* data, size_t offset, size_t count);
        void Use() const;
        size_t Id() const;

    private:

        mono::BufferTarget m_target;
        mono::BufferType m_type;
        mono::BufferData m_data;
        unsigned int m_buffer_id;
    };
}
