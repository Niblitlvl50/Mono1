//
//  Texture.h
//  Monolith1
//
//  Created by Niblit on 2011-12-10.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#pragma once

#include "ITexture.h"
#include "IImage.h"

namespace mono
{
    // Concrete implementation of ITexture interface
    class Texture : public ITexture
    {
    public:

        enum class TextureMode
        {
            REPEAT,
            CLAMP
        };
        
        Texture(const IImagePtr& image, TextureMode mode);
        ~Texture();
        
    private:
        virtual void Use() const;
        virtual unsigned int Id() const;
        virtual unsigned int Width() const;
        virtual unsigned int Height() const;
        
        unsigned int mTextureId;
        unsigned int mWidth;
        unsigned int mHeight;
    };
}

