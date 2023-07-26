
#pragma once

#include "Rendering/Texture/ITextureFactory.h"
#include <memory>

namespace mono
{
    class IImGuiImage
    {
    public:

        virtual ~IImGuiImage() = default;
        virtual void* TextureHandle() const = 0;
    };

    using IImGuiImagePtr = std::shared_ptr<IImGuiImage>;
    IImGuiImagePtr CreateImGuiImage(mono::ITexturePtr texture);
}
