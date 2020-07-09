
#include "FrameBufferCD.h"
#include "Texture/ITextureFactory.h"
#include "Texture/ITexture.h"
#include "System/open_gl.h"
#include "System/System.h"

using namespace mono;

FrameBuffer::FrameBuffer(int width, int height)
    : m_size(width, height)
{
    glGenFramebuffers(1, &m_fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);

    m_texture = mono::GetTextureFactory()->CreateTexture(nullptr, width, height, 3);

    //glGenTextures(1, &m_fbo_texture);
    //glBindTexture(GL_TEXTURE_2D, m_fbo_texture);
    //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //glBindTexture(GL_TEXTURE_2D, 0);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_texture->Id(), 0);

    glGenRenderbuffers(1, &m_rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, m_rbo); 
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_rbo);

    const GLenum result = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if(result != GL_FRAMEBUFFER_COMPLETE)
    {
        System::Log("Failed to create frame buffer.");
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

FrameBuffer::~FrameBuffer()
{
    glDeleteFramebuffers(1, &m_fbo);
}

const math::Vector& FrameBuffer::Size() const
{
    return m_size;
}

mono::ITexturePtr FrameBuffer::Texture() const
{
    return m_texture;
}

void FrameBuffer::Use()
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
}

void mono::ClearFrameBuffer()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}