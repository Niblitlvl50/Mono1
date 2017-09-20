
#pragma once

#include "RenderFwd.h"
#include <memory>

namespace mono
{
    using ICameraPtr = std::shared_ptr<ICamera>;
    using IDrawablePtr = std::shared_ptr<IDrawable>;
    using IRendererPtr = std::shared_ptr<IRenderer>;
    using IShaderPtr = std::unique_ptr<IShader>;
    using ISpritePtr = std::shared_ptr<ISprite>;
    using ITexturePtr = std::shared_ptr<ITexture>;
}
