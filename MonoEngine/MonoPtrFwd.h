//
//  MonoPtrFwd.h
//  Worksample_2
//
//  Created by Niklas Damberg on 07/11/13.
//
//

#pragma once

#include "MonoFwd.h"
#include <memory>

namespace mono
{
    typedef std::shared_ptr<IWindow> IWindowPtr;
    typedef std::shared_ptr<ICamera> ICameraPtr;
    typedef std::shared_ptr<IRenderer> IRendererPtr;
    typedef std::shared_ptr<IDrawable> IDrawablePtr;
    typedef std::shared_ptr<IEntity> IEntityPtr;
    typedef std::shared_ptr<IPhysicsEntity> IPhysicsEntityPtr;
    typedef std::shared_ptr<IUpdatable> IUpdatablePtr;
    typedef std::shared_ptr<IInputHandler> IInputHandlerPtr;
    typedef std::shared_ptr<IZone> IZonePtr;
    typedef std::shared_ptr<ITexture> ITexturePtr;
    typedef std::shared_ptr<IAction> IActionPtr;
}
