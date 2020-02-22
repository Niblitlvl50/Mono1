
#pragma once

#include "ITexture.h"

namespace mono
{
    class Texture : public ITexture
    {
    public:

        Texture(uint32_t width, uint32_t height, uint32_t color_components, const unsigned char* data);
        ~Texture();

        void Use() const override;
        uint32_t Id() const override;
        uint32_t Width() const override;
        uint32_t Height() const override;

    private:

        uint32_t m_texture_id;
        uint32_t m_width;
        uint32_t m_height;
    };
}

