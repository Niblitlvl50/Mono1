/*
 *  IView.h
 *  Monolith1
 *
 *  Created by Niblit on 2011-02-08.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef MONO_IWINDOW_H
#define MONO_IWINDOW_H

#include "MonoFwd.h"

namespace mono
{
	
	struct IWindow
	{
		virtual ~IWindow()
        { }
		
		virtual void SurfaceChanged(int width, int height) = 0;
		virtual void Activated(bool activated) = 0;
		virtual void DrawFrame(IRenderer& renderer) const = 0;
	};

}


#endif

