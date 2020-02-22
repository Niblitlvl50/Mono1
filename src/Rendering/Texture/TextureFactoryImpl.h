
#pragma once

#include "ITextureFactory.h"

namespace mono
{
    class TextureFactoryImpl : public ITextureFactory
    {
    public:

        ITexturePtr CreateTexture(const char* texture_name) const override;
        ITexturePtr CreateTextureFromData(const byte* data, int data_length, const char* cache_name) const override;
        ITexturePtr CreateTexture(const byte* data, int width, int height, int color_components) const override;
    };
}
