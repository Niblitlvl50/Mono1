
#pragma once

#include <memory>

using byte = unsigned char;

namespace mono
{
    using ITexturePtr = std::shared_ptr<class ITexture>;

    //! Create a texture from disk, only PNG is supported at the moment
    ITexturePtr CreateTexture(const char* texture_name);

    // 'cache_name' can be nullptr, then it will not be stored in the cache.
    ITexturePtr CreateTextureFromData(const byte* data, int data_length, const char* cache_name);

    //! Create a texture from memory
    ITexturePtr CreateTexture(const byte* data, int width, int height, int color_components);
}
