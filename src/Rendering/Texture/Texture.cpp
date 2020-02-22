
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

Texture::Texture(uint32_t width, uint32_t height, uint32_t color_components, const unsigned char* data)
    : m_texture_id(-1)
    , m_width(width)
    , m_height(height)
{
    width = NextPowerOfTwo(width);
    height = NextPowerOfTwo(height);

    GLenum format = GL_ALPHA;
    if(color_components == 3)
        format = GL_RGB;
    else if(color_components == 4)
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
