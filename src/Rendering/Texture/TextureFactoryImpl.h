
#pragma once

#include "ITextureFactory.h"
#include <unordered_map>
#include <cstdint>

namespace mono
{
    class TextureFactoryImpl : public ITextureFactory
    {
    public:

        ITexturePtr CreateTexture(const char* texture_name) const override;
        ITexturePtr CreateTextureFromData(const byte* data, int data_length, const char* cache_name) const override;
        ITexturePtr CreateTexture(const byte* data, int width, int height, int color_components) const override;
        ITexturePtr CreateFromNativeHandle(uint32_t native_handle) const override;

    private:

        mono::ITexturePtr GetTextureFromCache(uint32_t texture_hash) const;
        mono::ITexturePtr CreateAndCacheTexture(const char* source_file, uint32_t texture_hash) const;
        mono::ITexturePtr CreateAndCacheTexture(const unsigned char* data, int data_length, uint32_t texture_hash) const;

        // This is where all the weak pointers goes, that points to the allocated textures!
        mutable std::unordered_map<uint32_t, std::weak_ptr<mono::ITexture>> m_texture_storage;
    };
}
