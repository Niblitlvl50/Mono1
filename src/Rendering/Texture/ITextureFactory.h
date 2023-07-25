
#pragma once

#include <memory>

using byte = unsigned char;

namespace mono
{
    using ITexturePtr = std::shared_ptr<class ITexture>;
    using ISamplerPtr = std::unique_ptr<class ISampler>;

    class ITextureFactory
    {
    public:
    
        virtual ~ITextureFactory() = default;

        //! Create a texture from disk, only PNG is supported at the moment
        virtual ITexturePtr CreateTexture(const char* texture_name) const = 0;

        // 'cache_name' can be nullptr, then it will not be stored in the cache.
        virtual ITexturePtr CreateTextureFromData(const byte* data, int data_length, const char* cache_name) const = 0;

        //! Create a texture from memory
        virtual ITexturePtr CreateTexture(const byte* data, int width, int height, int color_components) const = 0;

        virtual ITexturePtr CreateFromNativeHandle(uint32_t native_handle) const = 0;

        virtual ISamplerPtr CreateSampler() const = 0;
    };
}
