
#pragma once

#include "IRenderBuffer.h"
#include "BufferFactory.h"

namespace mono
{
    class RenderBuffer : public IRenderBuffer
    {
    public:
    
        RenderBuffer(BufferType type, BufferData data, uint count);
        ~RenderBuffer();

        void UpdateData(const void* data, uint offset, uint count);
        void Use() const;
        uint Id() const;

    private:

        BufferType m_type;
        BufferData m_data;
        uint m_buffer_id;
    };
}
