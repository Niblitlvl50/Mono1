
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

        void UpdateData(const void* data, size_t offset, size_t count) override;
        void Use() const override;
        uint32_t Id() const override;

    private:

        mono::BufferTarget m_target;
        mono::BufferType m_type;
        mono::BufferData m_data;
        uint32_t m_buffer_id;
    };
}
