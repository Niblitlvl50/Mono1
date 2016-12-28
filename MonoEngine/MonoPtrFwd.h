
#pragma once

#include "MonoFwd.h"
#include <memory>

namespace mono
{
    using IWindowPtr = std::shared_ptr<IWindow>;
    using ICameraPtr = std::shared_ptr<ICamera>;
    using IRendererPtr = std::shared_ptr<IRenderer>;
    using IDrawablePtr = std::shared_ptr<IDrawable>;
    using IEntityPtr = std::shared_ptr<IEntity>;
    using IPhysicsEntityPtr = std::shared_ptr<IPhysicsEntity>;
    using IUpdatablePtr = std::shared_ptr<IUpdatable>;
    using IInputHandlerPtr = std::shared_ptr<IInputHandler>;
    using IZonePtr = std::shared_ptr<IZone>;
    using ITexturePtr = std::shared_ptr<ITexture>;
    using IActionPtr = std::shared_ptr<IAction>;
    using ISoundPtr = std::shared_ptr<ISound>;
    using IShaderPtr = std::shared_ptr<IShader>;
    using ISpritePtr = std::shared_ptr<ISprite>;
    using IPathPtr = std::shared_ptr<IPath>;
}
