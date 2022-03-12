
#pragma once

#include "Rendering/RenderFwd.h"
#include <memory>

namespace mono
{
    struct TextureDrawBuffers
    {
        std::unique_ptr<IRenderBuffer> vertices;
        std::unique_ptr<IRenderBuffer> uv;
        std::unique_ptr<IElementBuffer> indices;
    };

    TextureDrawBuffers BuildTextureDrawBuffers(const mono::ITexture* texture);
}
