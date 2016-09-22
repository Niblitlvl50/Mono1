//
//  Texture.h
//  Monolith1
//
//  Created by Niblit on 2011-12-10.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#pragma once

#include "ITexture.h"
#include "System/IImage.h"

namespace mono
{
    // Concrete implementation of ITexture interface
    class Texture : public ITexture
    {
    public:

        Texture(const IImagePtr& image);
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

