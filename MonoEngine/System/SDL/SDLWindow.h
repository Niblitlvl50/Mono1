/*
 *  SDLView.h
 *  Monolith1
 *
 *  Created by Niblit on 2011-02-08.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */


#pragma once

#include "IWindow.h"
#include "Vector2f.h"
#include <string>

struct SDL_Window;

namespace mono
{
    class SDLWindow : public mono::IWindow
    {
    public:
        
        SDLWindow(const std::string& title, int width, int height, bool fullscreen);
        virtual ~SDLWindow();

        virtual void SurfaceChanged(int width, int height);
        virtual void Activated(bool activated);
        virtual void MakeCurrent();
        virtual void SwapBuffers() const;
        virtual void SetBackgroundColor(const mono::Color& color);
        virtual const math::Vector2f& Size() const;
	
    private:
                
        math::Vector2f mSize;
        SDL_Window* mWindow;
        void* mContext;
    };
}

