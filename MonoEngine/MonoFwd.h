/*
 *  MonolithFwd.h
 *  Monolith1
 *
 *  Created by Niblit on 2011-02-08.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once

#include <tr1/memory>
#include <vector>
#include <string>

typedef unsigned char byte;

namespace mono
{
    struct IWindow;
    typedef std::tr1::shared_ptr<IWindow> IWindowPtr;

    struct ICamera;
    typedef std::tr1::shared_ptr<ICamera> ICameraPtr;

    struct IOGLSetup;
    typedef std::tr1::shared_ptr<IOGLSetup> IOGLSetupPtr;
	
    struct IRenderer;
    typedef std::tr1::shared_ptr<IRenderer> IRendererPtr;
	
    struct IDrawable;
    typedef std::tr1::shared_ptr<IDrawable> IDrawablePtr;
    typedef std::vector<IDrawablePtr> IDrawableCollection;

    struct IEntity;
    typedef std::tr1::shared_ptr<IEntity> IEntityPtr;
    typedef std::vector<IEntityPtr> IEntityCollection;

    struct IUpdatable;
    typedef std::tr1::shared_ptr<IUpdatable> IUpdatablePtr;
    typedef std::vector<IUpdatablePtr> IUpdatableCollection;

    struct IInputHandler;
    typedef std::tr1::shared_ptr<IInputHandler> IInputHandlerPtr;

    struct IZone;
    typedef std::tr1::shared_ptr<IZone> IZonePtr;

    struct ITexture;
    typedef std::tr1::shared_ptr<ITexture> ITexturePtr;
}

namespace Math
{
    struct Vector2f;
    struct Quad;
}


