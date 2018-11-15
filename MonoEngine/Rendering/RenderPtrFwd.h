
#pragma once

#include "RenderFwd.h"
#include <memory>

namespace mono
{
    using ICameraPtr = std::shared_ptr<ICamera>;
    using IDrawablePtr = std::shared_ptr<IDrawable>;
    using ISpritePtr = std::unique_ptr<ISprite>;
    using ITexturePtr = std::shared_ptr<ITexture>;
}
