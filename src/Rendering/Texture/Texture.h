
#pragma once

#include "ITexture.h"
#include "IImage.h"

namespace mono
{
    // Concrete implementation of ITexture interface
    class Texture : public ITexture
    {
    public:

        Texture(const mono::IImagePtr& image);
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

