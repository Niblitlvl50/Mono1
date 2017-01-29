
#include "RenderBuffer.h"
#include "System/SysOpenGL.h"

using namespace mono;

RenderBuffer::RenderBuffer(BufferType type, BufferData data, uint count)
    : m_type(type),
      m_data(data)
{
    glGenBuffers(1, &m_buffer_id);
    Use();

    const uint size = (m_data == BufferData::FLOAT) ? sizeof(float) : sizeof(int);
    const GLenum usage = (m_type == BufferType::STATIC) ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW;

    glBufferData(GL_ARRAY_BUFFER, size * count, nullptr, usage);
}

RenderBuffer::~RenderBuffer()
{
    glDeleteBuffers(1, &m_buffer_id);
}

void RenderBuffer::UpdateData(const void* data, uint offset, uint count)
{
    Use();

    const uint size = (m_data == BufferData::FLOAT) ? sizeof(float) : sizeof(int);
    glBufferSubData(GL_ARRAY_BUFFER, offset, size * count, data);
}

void RenderBuffer::Use() const
{
    glBindBuffer(GL_ARRAY_BUFFER, m_buffer_id);
}

uint RenderBuffer::Id() const
{
    return m_buffer_id;
}

void mono::ClearRenderBuffer()
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
