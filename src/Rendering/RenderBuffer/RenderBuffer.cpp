
#include "RenderBuffer.h"
#include "Rendering/ErrorChecking.h"
#include "System/open_gl.h"

using namespace mono;

constexpr GLenum usage_type_lookup[] = {
    GL_STATIC_DRAW,
    GL_DYNAMIC_DRAW
};

RenderBuffer::RenderBuffer(mono::BufferType type, mono::BufferData data, int components, size_t count)
    : m_type(type)
    , m_data(data)
    , m_components(components)
    , m_size(count)
{
    glGenBuffers(1, &m_buffer_id);
    Use();

    const GLenum buffer_usage = usage_type_lookup[static_cast<int>(m_type)];
    const size_t size = (m_data == BufferData::FLOAT) ? sizeof(float) : sizeof(int);
    glBufferData(GL_ARRAY_BUFFER, size * count, nullptr, buffer_usage);

    PROCESS_GL_ERRORS();
}

RenderBuffer::~RenderBuffer()
{
    glDeleteBuffers(1, &m_buffer_id);
}

void RenderBuffer::UpdateData(const void* data, uint32_t offset, uint32_t count)
{
    Use();

    const size_t size = (m_data == BufferData::FLOAT) ? sizeof(float) : sizeof(int);
    glBufferSubData(GL_ARRAY_BUFFER, offset * size, size * count, data);

    PROCESS_GL_ERRORS();
}

void RenderBuffer::Use() const
{
    glBindBuffer(GL_ARRAY_BUFFER, m_buffer_id);
    PROCESS_GL_ERRORS();
}

int RenderBuffer::Components() const
{
    return m_components;
}

uint32_t RenderBuffer::Size() const
{
    return m_size;
}

uint32_t RenderBuffer::SizeInBytes() const
{
    const size_t data_size = (m_data == BufferData::FLOAT) ? sizeof(float) : sizeof(int);
    return m_size * data_size;
}

uint32_t RenderBuffer::Id() const
{
    return m_buffer_id;
}


ElementBuffer::ElementBuffer(mono::BufferType type, mono::BufferData data, size_t count)
    : m_type(type)
    , m_data(data)
    , m_size(count)
{
    glGenBuffers(1, &m_buffer_id);
    Use();

    const GLenum buffer_usage = usage_type_lookup[static_cast<int>(m_type)];
    const size_t size = (m_data == BufferData::FLOAT) ? sizeof(float) : sizeof(int);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size * count, nullptr, buffer_usage);
    
    PROCESS_GL_ERRORS();
}

ElementBuffer::~ElementBuffer()
{
    glDeleteBuffers(1, &m_buffer_id);
}

void ElementBuffer::UpdateData(const void* data, uint32_t offset, uint32_t count)
{
    Use();

    const size_t size = (m_data == BufferData::FLOAT) ? sizeof(float) : sizeof(int);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset * size, size * count, data);

    PROCESS_GL_ERRORS();
}

void ElementBuffer::Use() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_buffer_id);
    PROCESS_GL_ERRORS();
}

uint32_t ElementBuffer::Size() const
{
    return m_size;
}

uint32_t ElementBuffer::Id() const
{
    return m_buffer_id;
}


void mono::ClearRenderBuffer()
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
