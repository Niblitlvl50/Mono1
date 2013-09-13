/*
 *  MonolithFwd.h
 *  Monolith1
 *
 *  Created by Niblit on 2011-02-08.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once

#include <memory>
#include <vector>
#include <string>
#include "SysTypes.h"

namespace mono
{
    struct IWindow;
    typedef std::shared_ptr<IWindow> IWindowPtr;

    struct ICamera;
    typedef std::shared_ptr<ICamera> ICameraPtr;
	
    struct IRenderer;
    typedef std::shared_ptr<IRenderer> IRendererPtr;
	
    struct IDrawable;
    typedef std::shared_ptr<IDrawable> IDrawablePtr;
    typedef std::vector<IDrawablePtr> IDrawableCollection;

    struct IEntity;
    typedef std::shared_ptr<IEntity> IEntityPtr;
    typedef std::vector<IEntityPtr> IEntityCollection;
    
    struct IPhysicsEntity;
    typedef std::shared_ptr<IPhysicsEntity> IPhysicsEntityPtr;

    struct IUpdatable;
    typedef std::shared_ptr<IUpdatable> IUpdatablePtr;
    typedef std::vector<IUpdatablePtr> IUpdatableCollection;

    struct IInputHandler;
    typedef std::shared_ptr<IInputHandler> IInputHandlerPtr;

    struct IZone;
    typedef std::shared_ptr<IZone> IZonePtr;

    struct ITexture;
    typedef std::shared_ptr<ITexture> ITexturePtr;
}
