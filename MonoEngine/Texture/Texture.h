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

    class Texture : public ITexture
    {
    public:
        
        Texture(IImagePtr image);
        ~Texture();
        
        static void Clear();

    private:
        virtual void Use() const;
        virtual int Width() const;
        virtual int Height() const;
        
        unsigned int mTextureId;
        unsigned int mWidth;
        unsigned int mHeight;
        
        static unsigned int sBoundTexture;
    };
    
}

