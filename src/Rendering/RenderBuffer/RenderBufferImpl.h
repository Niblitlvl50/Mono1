
#pragma once

#include "IRenderBuffer.h"
#include "BufferFactory.h"
#include "sokol/sokol_gfx.h"

namespace mono
{
    class RenderBufferImpl : public mono::IRenderBuffer
    {
    public:

        RenderBufferImpl(mono::BufferType buffer_type, mono::BufferData data_type, uint32_t components, uint32_t count, const void* data_ptr);
        ~RenderBufferImpl();
        void UpdateData(const void* data, uint32_t offset, uint32_t count) override;
        uint32_t Size() const override;
        uint32_t Id() const override;

        mono::BufferData m_data_type;
        uint32_t m_components;
        uint32_t m_count;
        sg_buffer m_handle;
    };

    class IndexBufferImpl : public mono::IElementBuffer
    {
    public:

        IndexBufferImpl(mono::BufferType buffer_type, uint32_t count, const void* data_ptr);
        ~IndexBufferImpl();
        void UpdateData(const void* data, uint32_t offset, uint32_t count) override;
        uint32_t Size() const override;
        uint32_t Id() const override;

        uint32_t m_count;
        sg_buffer m_handle;
    };
}
