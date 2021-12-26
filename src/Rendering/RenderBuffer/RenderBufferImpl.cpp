
#include "RenderBufferImpl.h"
#include "System/System.h"

using namespace mono;

namespace
{
    constexpr size_t data_type_sizes[] = {
        sizeof(int32_t),
        sizeof(int16_t),
        sizeof(float),
    };

    uint32_t CalculateByteSize(mono::BufferData data_type, uint32_t components, uint32_t count)
    {
        return data_type_sizes[(int)data_type] * components * count;
    }
}

RenderBufferImpl::RenderBufferImpl(
    mono::BufferType buffer_type, mono::BufferData data_type, uint32_t components, uint32_t count, const void* data_ptr)
    : m_data_type(data_type)
    , m_components(components)
    , m_count(count)
{
    sg_buffer_desc buffer_desc = {};
    buffer_desc.size = CalculateByteSize(m_data_type, m_components, count);
    buffer_desc.type = SG_BUFFERTYPE_VERTEXBUFFER;
    buffer_desc.usage = (buffer_type == mono::BufferType::STATIC) ? SG_USAGE_IMMUTABLE : SG_USAGE_DYNAMIC;
    buffer_desc.data = { data_ptr, buffer_desc.size };

    m_handle = sg_make_buffer(&buffer_desc);

    const sg_resource_state state = sg_query_buffer_state(m_handle);
    if(state != SG_RESOURCESTATE_VALID)
        System::Log("Failed to create render buffer");
}

RenderBufferImpl::~RenderBufferImpl()
{
    sg_destroy_buffer(m_handle);
}

void RenderBufferImpl::UpdateData(const void* data_ptr, uint32_t offset, uint32_t count)
{
    const uint32_t data_size = CalculateByteSize(m_data_type, m_components, count);
    sg_update_buffer(m_handle, { data_ptr, data_size });
}

uint32_t RenderBufferImpl::Size() const
{
    return m_count;
}

uint32_t RenderBufferImpl::ByteOffsetToIndex(uint32_t index) const
{
    return CalculateByteSize(m_data_type, m_components, index);
}

uint32_t RenderBufferImpl::Id() const
{
    return m_handle.id;
}


IndexBufferImpl::IndexBufferImpl(mono::BufferType buffer_type, uint32_t count, const uint16_t* data_ptr)
    : m_count(count)
{
    sg_buffer_desc buffer_desc = {};
    buffer_desc.size = CalculateByteSize(mono::BufferData::INT_16, 1, count);
    buffer_desc.type = SG_BUFFERTYPE_INDEXBUFFER;
    buffer_desc.usage = (buffer_type == mono::BufferType::STATIC) ? SG_USAGE_IMMUTABLE : SG_USAGE_DYNAMIC;
    buffer_desc.data = { data_ptr, buffer_desc.size };

    m_handle = sg_make_buffer(&buffer_desc);

    const sg_resource_state state = sg_query_buffer_state(m_handle);
    if(state != SG_RESOURCESTATE_VALID)
        System::Log("Failed to create index buffer");
}

IndexBufferImpl::~IndexBufferImpl()
{
    sg_destroy_buffer(m_handle);
}

void IndexBufferImpl::UpdateData(const uint16_t* data_ptr, uint32_t offset, uint32_t count)
{
    sg_update_buffer(m_handle, { data_ptr, CalculateByteSize(mono::BufferData::INT_16, 1, count) });
}

uint32_t IndexBufferImpl::Size() const
{
    return m_count;
}

uint32_t IndexBufferImpl::Id() const
{
    return m_handle.id;
}
