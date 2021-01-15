
#include "FrameBufferCD.h"
#include "Rendering/Texture/ITextureFactory.h"
#include "Rendering/Texture/ITexture.h"
#include "Rendering/RenderSystem.h"
#include "System/open_gl.h"
#include "System/System.h"
#include "Rendering/ErrorChecking.h"

#include <cstring>

using namespace mono;

FrameBuffer::FrameBuffer(int width, int height)
    : m_size(width, height)
{
    glGenFramebuffers(1, &m_fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);

    m_texture = mono::GetTextureFactory()->CreateTexture(nullptr, width, height, 3);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_texture->Id(), 0);

    glGenRenderbuffers(1, &m_rbo_depth_stencil);
    glBindRenderbuffer(GL_RENDERBUFFER, m_rbo_depth_stencil); 
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    glGenRenderbuffers(1, &m_rbo_color);
    glBindRenderbuffer(GL_RENDERBUFFER, m_rbo_color);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA8, width, height);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_rbo_depth_stencil);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_RENDERBUFFER, m_rbo_color);

    GLenum bufs[] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1};
    glDrawBuffers(2, bufs);

    const GLenum result = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if(result != GL_FRAMEBUFFER_COMPLETE)
    {
        System::Log("Failed to create frame buffer.");
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);


    const uint32_t pbo_size = width * height * 4; // * sizeof(byte);

    glGenBuffers(1, &m_pbo);
    glBindBuffer(GL_PIXEL_PACK_BUFFER, m_pbo);
    glBufferData(GL_PIXEL_PACK_BUFFER, pbo_size, nullptr, GL_STREAM_READ);
    glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);
}

FrameBuffer::~FrameBuffer()
{
    glDeleteFramebuffers(1, &m_fbo);
    glDeleteRenderbuffers(1, &m_rbo_depth_stencil);
    glDeleteRenderbuffers(1, &m_rbo_color);

    glDeleteBuffers(1, &m_pbo);
}

const math::Vector& FrameBuffer::Size() const
{
    return m_size;
}

mono::ITexturePtr FrameBuffer::Texture() const
{
    return m_texture;
}

bool FrameBuffer::CopyFrameBufferData(byte* data, uint32_t data_length)
{
    glBindFramebuffer(GL_READ_FRAMEBUFFER, m_fbo);
    glReadBuffer(GL_COLOR_ATTACHMENT1);
    
    glBindBuffer(GL_PIXEL_PACK_BUFFER, m_pbo);
    glReadPixels(0, 0, m_size.x, m_size.y, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    const byte* src = (const byte*)glMapBuffer(GL_PIXEL_PACK_BUFFER, GL_READ_ONLY);
    if(src != nullptr)
    {
        std::memcpy(data, src, data_length);
        glUnmapBuffer(GL_PIXEL_PACK_BUFFER);
    }

    glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);
    return src != nullptr;
}

void FrameBuffer::Use()
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
}

void mono::ClearFrameBuffer()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}