/*
 *  SDLView.h
 *  Monolith1
 *
 *  Created by Niblit on 2011-02-08.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */


#ifndef MONO_SDLWINDOW_H
#define MONO_SDLWINDOW_H

#include "IWindow.h"
#include "MonoFwd.h"

struct SDL_Window;

namespace mono
{

	class SDLWindow : public mono::IWindow
	{
	public:
		SDLWindow(const std::string& title, int width, int height, unsigned int flags, const IOGLSetupPtr setup);

		virtual void SurfaceChanged(int width, int height);
		virtual void Activated(bool activated);
		virtual void DrawFrame(IRenderer& renderer) const;
	
	private:
		
        IOGLSetupPtr mOGLSetup;
		        
        SDL_Window* mWindow;
        void* mContext;
	};
}


#endif

