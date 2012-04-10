/*
 *  SDLView.cpp
 *  Monolith1
 *
 *  Created by Niblit on 2011-02-08.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "SDLWindow.h"
#include "IRenderer.h"
#include "IOGLSetup.h"

#include <stdexcept>

#include <SDL_video.h>
#include <SDL_opengl.h>


using namespace mono;


SDLWindow::SDLWindow(const std::string& title, int width, int height, unsigned int flags, IOGLSetupPtr setup)
    : mOGLSetup(setup)
{
    
    /* Request opengl 2.1 context.
     * SDL doesn't have the ability to choose which profile at this time of writing,
     * but it should default to the core profile */
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    
    // Request double-buffered OpenGL
    // The fact that windows are double-buffered on Mac OS X has no effect
    // on OpenGL double buffering.
    const int doubleBuffer = 1; // == true, in this case
    if(SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, doubleBuffer) != 0)
		throw std::runtime_error("Unable to set doubel buffer gl attribute");
    
    // Request a 16-bit depth buffer (without this, there is no depth buffer)
    const int depthValue = 16;
    if(SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, depthValue) != 0)
		throw std::runtime_error("Unable to set depth size gl attribute");

    // Create our window centered and with the given resolution
    mWindow = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, flags);
    if(!mWindow)
        throw std::runtime_error("Unable to create sdl window");

    mContext = SDL_GL_CreateContext(mWindow);
    
    SDL_GL_SetSwapInterval(1);
    	    
    mOGLSetup->OnCreated();
    mOGLSetup->OnChanged(width, height);
}

void SDLWindow::SurfaceChanged(int width, int height)
{
    mOGLSetup->OnChanged(width, height);
}

void SDLWindow::Activated(bool activated)
{

}

void SDLWindow::DrawFrame(IRenderer& renderer) const
{
	renderer.DrawFrame();
    SDL_GL_SwapWindow(mWindow);
}



