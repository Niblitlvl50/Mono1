
#include "RenderBuffer.h"
#include "ErrorChecking.h"
#include "System/open_gl.h"

using namespace mono;

constexpr GLenum buffer_target_lookup[] = {
    GL_ARRAY_BUFFER,
    GL_ELEMENT_ARRAY_BUFFER
};

constexpr GLenum usage_type_lookup[] = {
    GL_STATIC_DRAW,
    GL_DYNAMIC_DRAW
};

RenderBuffer::RenderBuffer(mono::BufferTarget target, mono::BufferType type, mono::BufferData data, size_t count)
    : m_target(target)
    , m_type(type)
    , m_data(data)
{
    const GLenum buffer_target = buffer_target_lookup[static_cast<int>(m_target)];
    const GLenum buffer_usage = usage_type_lookup[static_cast<int>(m_type)];

    const size_t size = (m_data == BufferData::FLOAT) ? sizeof(float) : sizeof(int);

    glGenBuffers(1, &m_buffer_id);
    Use();
    glBufferData(buffer_target, size * count, nullptr, buffer_usage);
    
    PROCESS_GL_ERRORS();
}

RenderBuffer::~RenderBuffer()
{
    glDeleteBuffers(1, &m_buffer_id);
}

void RenderBuffer::UpdateData(const void* data, uint32_t offset, uint32_t count)
{
    const size_t size = (m_data == BufferData::FLOAT) ? sizeof(float) : sizeof(int);

    Use();

    const GLenum target = buffer_target_lookup[static_cast<int>(m_target)];
    glBufferSubData(target, offset * size, size * count, data);

    PROCESS_GL_ERRORS();
}

void RenderBuffer::Use() const
{
    const GLenum target = buffer_target_lookup[static_cast<int>(m_target)];
    glBindBuffer(target, m_buffer_id);
    PROCESS_GL_ERRORS();
}

uint32_t RenderBuffer::Id() const
{
    return m_buffer_id;
}

void mono::ClearRenderBuffer()
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
