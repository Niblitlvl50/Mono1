/*
 *  SDLView.h
 *  Monolith1
 *
 *  Created by Niblit on 2011-02-08.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */


#ifndef SDLVIEW_H
#define SDLVIEW_H

#include "IWindow.h"
#include "MonoFwd.h"
//#include "EventFwd.h"

#include <SDL_video.h>
#include <SDL_opengl.h>

namespace mono
{

	class SDLWindow : public mono::IWindow
	{
	public:
		SDLWindow(int width, int height, unsigned int flags, const IOGLSetupPtr setup);

		virtual void SurfaceChanged(int width, int height);
		virtual void Activated(bool activated);
		virtual void DrawFrame(IRenderer& renderer) const;
	
	private:
		
		int mWidth;
		int mHeight;
		unsigned int mFlags;
        IOGLSetupPtr mOGLSetup;
		        
        SDL_Window* mWindow;
        SDL_GLContext mContext;
	};
}


#endif

