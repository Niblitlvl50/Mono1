
#pragma once

#include "ITexture.h"
#include "sokol/sokol_gfx.h"

namespace mono
{
    class TextureImpl : public ITexture
    {
    public:

        TextureImpl(
            uint32_t width,
            uint32_t height,
            uint32_t color_components,
            const unsigned char* image_data);
        TextureImpl(sg_image image_handle);
        ~TextureImpl();

        uint32_t Id() const override;
        uint32_t Width() const override;
        uint32_t Height() const override;
        bool IsAlphaTexture() const override;

        uint32_t m_width;
        uint32_t m_height;
        sg_pixel_format m_pixel_format;
        sg_image m_handle;
    };

    class SamplerImpl : public ISampler
    {
    public:
    
        SamplerImpl();
        ~SamplerImpl();
        uint32_t Id() const override;

        sg_sampler m_handle;
    };
}

