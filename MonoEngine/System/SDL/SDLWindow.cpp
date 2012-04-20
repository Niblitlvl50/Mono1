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
#include "SysOpenGL.h"
#include <stdexcept>

#include <SDL_video.h>
#include <SDL_opengl.h>


using namespace mono;

namespace
{
    void SetupOpenGL()
    {
        const float ambient[] = { 0.2, 0.2, 0.2, 1.0 };
        const float diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
        const float position[] = { 0.0, 1.0, 0.0, 0.0 };
        const float mat_shininess[] = { 90.0 };
        const float mat_specular[] = { 0.8, 0.8, 0.8, 1.0 };
        const float mat_diffuse[] = { 0.46, 0.66, 0.795, 1.0 };
        const float mat_ambient[] = { 0.3, 0.4, 0.5, 1.0 };
        const float lmodel_ambient[] = { 0.4, 0.4, 0.4, 1.0 };
        const float lmodel_localviewer[] = { 0.0 };
        
        const float fog_color[] = { 0.0, 0.5, 0.9, 1.0 };
        
        glFrontFace(GL_CCW);
        
        glDepthFunc(GL_LESS);
        glEnable(GL_DEPTH_TEST);
        
        glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
        glLightfv(GL_LIGHT0, GL_POSITION, position);
        glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
        glLightModelfv(GL_LIGHT_MODEL_LOCAL_VIEWER, lmodel_localviewer);
        
        glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse);
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambient);
        
        glEnable(GL_FOG);
        glFogi(GL_FOG_MODE, GL_EXP);
        glFogf(GL_FOG_DENSITY, 0.0000025);
        glFogfv(GL_FOG_COLOR, fog_color);
        
        glEnable(GL_TEXTURE_2D);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        
        glClearColor(0.0, 0.5, 0.9, 1.0);
    }
    
    void SetProjection(int width, int height)
    {
        glViewport(0, 0, width, height);
        
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        
        glOrtho(0, width, 0, height, 0, 10);
        
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
    }
}


SDLWindow::SDLWindow(const std::string& title, int width, int height, bool fullscreen)
    : mWidth(width),
      mHeight(height)
{
    const unsigned int screenflag = fullscreen ? SDL_WINDOW_FULLSCREEN : SDL_WINDOW_RESIZABLE;
	const unsigned int flags = SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | screenflag;
    
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
    	        
    SetupOpenGL();
    SetProjection(width, height);
}

SDLWindow::~SDLWindow()
{
    SDL_GL_DeleteContext(mContext);
    SDL_DestroyWindow(mWindow);
}

void SDLWindow::SurfaceChanged(int width, int height)
{
    mWidth = width;
    mHeight = height;
    
    SetProjection(width, height);
}

void SDLWindow::Activated(bool activated)
{

}

void SDLWindow::DrawFrame(IRenderer& renderer) const
{
	renderer.DrawFrame();
    SDL_GL_SwapWindow(mWindow);
}

int SDLWindow::GetWidth() const
{
    return mWidth;
}

int SDLWindow::GetHeight() const
{
    return mHeight;
}



