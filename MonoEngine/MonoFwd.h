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

    struct IEntity;
    typedef std::shared_ptr<IEntity> IEntityPtr;
    
    struct IPhysicsEntity;
    typedef std::shared_ptr<IPhysicsEntity> IPhysicsEntityPtr;

    struct IUpdatable;
    typedef std::shared_ptr<IUpdatable> IUpdatablePtr;

    struct IInputHandler;
    typedef std::shared_ptr<IInputHandler> IInputHandlerPtr;

    struct IZone;
    typedef std::shared_ptr<IZone> IZonePtr;

    struct ITexture;
    typedef std::shared_ptr<ITexture> ITexturePtr;
    
    struct Color;
}
