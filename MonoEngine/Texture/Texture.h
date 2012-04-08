//
//  Texture.h
//  Monolith1
//
//  Created by Niblit on 2011-12-10.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#ifndef MONO_TEXTURE_H
#define MONO_TEXTURE_H

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
        
        unsigned int mTextureId;
        static unsigned int mBoundTexture;
    };
    
}

#endif

