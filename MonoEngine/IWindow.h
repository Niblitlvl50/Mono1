/*
 *  IView.h
 *  Monolith1
 *
 *  Created by Niblit on 2011-02-08.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once

#include "MathFwd.h"

namespace mono
{
    struct Color;
    
    struct IWindow
    {
        virtual ~IWindow()
        { }
		
        virtual void SurfaceChanged(int width, int height) = 0;
        virtual void Activated(bool activated) = 0;
        virtual void SwapBuffers() const = 0;
        virtual void MakeCurrent() = 0;
        virtual void SetBackgroundColor(const mono::Color& color) = 0;
        virtual const math::Vector2f& Size() const = 0;
    };

}

