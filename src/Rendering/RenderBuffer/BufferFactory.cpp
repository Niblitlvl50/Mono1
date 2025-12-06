
#include "BufferFactory.h"
#include "RenderBufferImpl.h"

std::unique_ptr<mono::IRenderBuffer> mono::CreateRenderBuffer(
    mono::BufferType buffer_type, mono::BufferData data_type, uint32_t components, uint32_t count, const void* data_ptr, const char* debug_label)
{
    return std::make_unique<mono::RenderBufferImpl>(buffer_type, data_type, components, count, data_ptr, debug_label);
}

std::unique_ptr<mono::IElementBuffer> mono::CreateElementBuffer(
    mono::BufferType buffer_type, uint32_t count, const uint16_t* data_ptr, const char* debug_label)
{
    return std::make_unique<mono::IndexBufferImpl>(buffer_type, count, data_ptr, debug_label);
}
