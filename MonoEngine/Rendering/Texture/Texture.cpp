
#include "Texture.h"
#include "System/open_gl.h"
#include <cstdio>

using namespace mono;

namespace
{
    unsigned int NextPowerOfTwo(unsigned int x)
    {
        unsigned int val = 1;
        while(val < x)
            val *= 2;

        return val;
    }
}

Texture::Texture(const System::IImagePtr& image)
    : mTextureId(-1),
      mWidth(image->Width()),
      mHeight(image->Height())
{
    const int components = image->ColorComponents();
    const unsigned int width = NextPowerOfTwo(image->Width());
    const unsigned int height = NextPowerOfTwo(image->Height());
    const byte* data = image->Data();

    GLenum format = GL_ALPHA;
    if(components == 3)
        format = GL_RGB;
    else if(components == 4)
        format = GL_RGBA;

    glGenTextures(1, &mTextureId);
    glBindTexture(GL_TEXTURE_2D, mTextureId);

    glTexImage2D(GL_TEXTURE_2D, 0, format, (int)width, (int)height, 0, format, GL_UNSIGNED_BYTE, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    const GLenum error = glGetError();
    if(error != GL_NO_ERROR)
        std::printf("OpenGL error in Texture. Error no: 0x%X\n", error);
}

Texture::~Texture()
{
    glDeleteTextures(1, &mTextureId);
}

void Texture::Use() const
{
    glBindTexture(GL_TEXTURE_2D, mTextureId);
}

unsigned int Texture::Id() const
{
    return mTextureId;
}

unsigned int Texture::Width() const
{
    return mWidth;
}

unsigned int Texture::Height() const
{
    return mHeight;
}

void mono::ClearTexture()
{
    glBindTexture(GL_TEXTURE_2D, 0);
}
