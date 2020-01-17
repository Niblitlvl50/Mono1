
#include "Texture.h"
#include "System/open_gl.h"
#include "System/System.h"
#include <cstdio>

using namespace mono;

namespace
{
    uint32_t NextPowerOfTwo(uint32_t x)
    {
        uint32_t val = 1;
        while(val < x)
            val *= 2;

        return val;
    }
}

Texture::Texture(const mono::IImagePtr& image)
    : m_texture_id(-1),
      m_width(image->Width()),
      m_height(image->Height())
{
    const int components = image->ColorComponents();
    const uint32_t width = NextPowerOfTwo(image->Width());
    const uint32_t height = NextPowerOfTwo(image->Height());
    const byte* data = image->Data();

    GLenum format = GL_ALPHA;
    if(components == 3)
        format = GL_RGB;
    else if(components == 4)
        format = GL_RGBA;

    glGenTextures(1, &m_texture_id);
    glBindTexture(GL_TEXTURE_2D, m_texture_id);

    glTexImage2D(GL_TEXTURE_2D, 0, format, (int)width, (int)height, 0, format, GL_UNSIGNED_BYTE, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    const GLenum error = glGetError();
    if(error != GL_NO_ERROR)
        System::Log("Texture|OpenGL error in Texture. Error no: 0x%X\n", error);
}

Texture::~Texture()
{
    glDeleteTextures(1, &m_texture_id);
}

void Texture::Use() const
{
    glBindTexture(GL_TEXTURE_2D, m_texture_id);
}

uint32_t Texture::Id() const
{
    return m_texture_id;
}

uint32_t Texture::Width() const
{
    return m_width;
}

uint32_t Texture::Height() const
{
    return m_height;
}

void mono::ClearTexture()
{
    glBindTexture(GL_TEXTURE_2D, 0);
}
