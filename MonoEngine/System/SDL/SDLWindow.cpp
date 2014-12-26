/*
 *  SDLView.cpp
 *  Monolith1
 *
 *  Created by Niblit on 2011-02-08.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "SDLWindow.h"
#include "SysOpenGL.h"
#include "Color.h"

#include <stdexcept>
#include <string>
#include <iostream>

#include "SDL.h"

using namespace mono;

namespace
{
    void PrintOpenGLInfo()
    {
        const GLubyte* vendor = glGetString(GL_VENDOR);
        if(vendor)
            std::cout << "Vendor: " << vendor << std::endl;

        const GLubyte* renderer = glGetString(GL_RENDERER);
        if(renderer)
            std::cout << "Renderer: " << renderer << std::endl;

        const GLubyte* version = glGetString(GL_VERSION);
        if(version)
            std::cout << "Version: " << version << std::endl;

        const GLubyte* glslversion = glGetString(GL_SHADING_LANGUAGE_VERSION);
        if(glslversion)
            std::cout << "GLSL version: " << glslversion << std::endl;

        /*
        const GLubyte* extensions = glGetString(GL_EXTENSIONS);
        if(extensions)
            std::cout << "Extensions: " << extensions << std::endl;
         */
    }

    void SetupOpenGL()
    {
        glFrontFace(GL_CCW);
        
        glDisable(GL_DEPTH_TEST);
                
        glEnable(GL_TEXTURE_2D);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        
        glEnable(GL_POINT_SMOOTH);
        glEnable(GL_LINE_SMOOTH);
        //glEnable(GL_POLYGON_SMOOTH);
    }
}


SDLWindow::SDLWindow(const std::string& title, int width, int height, bool fullscreen)
    : mSize(width, height),
      mWindow(nullptr),
      mContext(nullptr)
{
    /* Request opengl 2.1 context.
     * SDL doesn't have the ability to choose which profile at this time of writing,
     * but it should default to the core profile */
    //SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    //SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 5);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 6);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 5);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 0);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 0);
    SDL_GL_SetAttribute(SDL_GL_RETAINED_BACKING, 1);
    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

    //const unsigned int screenflag = fullscreen ? SDL_WINDOW_FULLSCREEN : SDL_WINDOW_RESIZABLE;
    const unsigned int flags = SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | SDL_WINDOW_BORDERLESS; // | screenflag;

    // Create our window centered and with the given resolution
    mWindow = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, flags);
    if(!mWindow)
        throw std::runtime_error("Unable to create sdl window");

    mContext = SDL_GL_CreateContext(mWindow);
    if(!mContext)
        throw std::runtime_error("Unable to create OpenGL context");

    //SDL_ShowCursor(SDL_DISABLE);
    	        
    PrintOpenGLInfo();
    SetupOpenGL();
    MakeCurrent();
}

SDLWindow::~SDLWindow()
{
    SDL_GL_DeleteContext(mContext);
    SDL_DestroyWindow(mWindow);
}

void SDLWindow::SurfaceChanged(int width, int height)
{
    mSize = math::Vector2f(width, height);
}

void SDLWindow::Activated(bool activated)
{ }

void SDLWindow::MakeCurrent()
{
    SDL_GL_MakeCurrent(mWindow, mContext);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void SDLWindow::SwapBuffers() const
{
    SDL_GL_SwapWindow(mWindow);
}

void SDLWindow::SetBackgroundColor(const mono::Color& color)
{
    glClearColor(color.red, color.green, color.blue, color.alpha);
}

const math::Vector2f& SDLWindow::Size() const
{
    return mSize;
}



