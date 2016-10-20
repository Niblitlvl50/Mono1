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
#include "Math/Vector2f.h"

struct SDL_Window;

namespace mono
{
    class SDLWindow : public mono::IWindow
    {
    public:
        
        SDLWindow(const char* title, int width, int height, bool fullscreen);
        virtual ~SDLWindow();

        virtual void SurfaceChanged(int width, int height);
        virtual void Activated(bool activated);
        virtual void Maximize();
        virtual void Minimize();
        virtual void RestoreSize();
        virtual void MakeCurrent();
        virtual void SwapBuffers() const;
        virtual void SetBackgroundColor(const mono::Color::RGBA& color);
        virtual const math::Vector2f& Size() const;
	
    private:
                
        math::Vector2f mSize;
        SDL_Window* mWindow;
        void* mContext;
    };
}

