
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
        void Use() const override;
        uint32_t Id() const override;
        uint32_t Width() const override;
        uint32_t Height() const override;
        
        uint32_t m_texture_id;
        uint32_t m_width;
        uint32_t m_height;
    };
}

