//
//  Texture.cpp
//  Monolith1
//
//  Created by Niblit on 2011-12-10.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#include "Texture.h"
#include "SysOpenGL.h"
#include <iostream>

using namespace mono;

namespace
{
    int NextPowerOfTwo(int x)
    {
        int val = 1;
        while(val < x)
            val *= 2;

        return val;
    }
}

unsigned int mono::Texture::sBoundTexture = -1;

Texture::Texture(IImagePtr image)
    : mTextureId(-1),
      mWidth(image->Width()),
      mHeight(image->Height())
{
    glGenTextures(1, &mTextureId);
    glBindTexture(GL_TEXTURE_2D, mTextureId);
    
    const int internalFormat = image->ColorComponents();
    const int width = NextPowerOfTwo(image->Width());
    const int height = NextPowerOfTwo(image->Height());
    const unsigned int sourceFormat = image->TargetFormat();
    const byte* data = image->Data();
    
    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, sourceFormat, GL_UNSIGNED_BYTE, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    
    const GLenum error = glGetError();
    if(error != GL_NO_ERROR)
        std::cout << "Open GL error in Texture. Error no: 0x" << std::hex << error << std::endl;
}

Texture::~Texture()
{
    glDeleteTextures(1, &mTextureId);
}

void Texture::Use() const
{
    if(sBoundTexture != mTextureId)
    {
        glBindTexture(GL_TEXTURE_2D, mTextureId);
        sBoundTexture = mTextureId;
    }
}

int Texture::Width() const
{
    return mWidth;
}

int Texture::Height() const
{
    return mHeight;
}

void Texture::Clear()
{
    glBindTexture(GL_TEXTURE_2D, 0);
    sBoundTexture = -1;
}
