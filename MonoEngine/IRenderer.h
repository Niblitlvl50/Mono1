/*
 *  IRenderer.h
 *  Monolith1
 *
 *  Created by Niblit on 2011-02-08.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once

#include "MonoFwd.h"

namespace mono
{

	struct IRenderer
	{
		virtual ~IRenderer()
        { }
		
		virtual void DrawFrame() const = 0;
        virtual void Update(unsigned int milliseconds) = 0;
        
        virtual void AddEntity(IEntityPtr entity) = 0;
        virtual void AddDrawable(IDrawablePtr drawable) = 0;
        virtual void AddUpdatable(IUpdatablePtr updatable) = 0;
	};
    
}

