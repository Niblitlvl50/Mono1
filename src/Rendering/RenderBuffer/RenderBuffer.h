
#pragma once

#include "IRenderBuffer.h"
#include "BufferFactory.h"

namespace mono
{
    class RenderBuffer : public IRenderBuffer
    {
    public:
    
        RenderBuffer(mono::BufferType type, mono::BufferData data, int components, size_t count);
        ~RenderBuffer();

        void UpdateData(const void* data, uint32_t offset, uint32_t count) override;
        void Use() const override;
        int Components() const override;
        uint32_t Size() const override;
        uint32_t SizeInBytes() const override;
        uint32_t Id() const override;

    private:

        mono::BufferType m_type;
        mono::BufferData m_data;
        int m_components;
        uint32_t m_size;
        uint32_t m_buffer_id;
    };


    class ElementBuffer : public IElementBuffer
    {
    public:
    
        ElementBuffer(mono::BufferType type, mono::BufferData data, size_t count);
        ~ElementBuffer();

        void UpdateData(const void* data, uint32_t offset, uint32_t count) override;
        void Use() const override;
        uint32_t Size() const override;
        uint32_t Id() const override;

    private:

        mono::BufferType m_type;
        mono::BufferData m_data;
        uint32_t m_size;
        uint32_t m_buffer_id;
    };
}
