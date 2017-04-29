
#pragma once

#include <memory>

namespace mono
{
    class ITexture;
    using ITexturePtr = std::shared_ptr<ITexture>;

    //! Create a texture from disk, only PNG is supported at the moment
    ITexturePtr CreateTexture(const char* source);

    //! Create a texture from memory
    ITexturePtr CreateTexture(const unsigned char* data, int width, int height, int colorComponents);
}
