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


unsigned int mono::Texture::mBoundTexture = -1;


Texture::Texture(IImagePtr image)
    : mTextureId(-1)
{
    glGenTextures(1, &mTextureId);
    glBindTexture(GL_TEXTURE_2D, mTextureId);
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image->Width(), image->Height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, image->Data());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    
    const GLenum error = glGetError();
    if(error != GL_NO_ERROR)
        std::cout << "Open GL error in Texture." << std::endl;
}

Texture::~Texture()
{
    glDeleteTextures(1, &mTextureId);
}

void Texture::Use() const
{
    if(mBoundTexture != mTextureId)
    {
        glBindTexture(GL_TEXTURE_2D, mTextureId);
        mBoundTexture = mTextureId;
    }
}

void Texture::Clear()
{
    glBindTexture(GL_TEXTURE_2D, 0);
    mBoundTexture = -1;
}
