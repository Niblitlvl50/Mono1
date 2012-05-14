/*
 *  MOGLRenderer.h
 *  Monolith1
 *
 *  Created by Niblit on 2011-02-08.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once

#include "IRenderer.h"

namespace mono
{

    class Renderer : public IRenderer
    {
    public:
        
        Renderer(ICameraPtr camera);
        
        virtual void DrawFrame() const;
        virtual void Update(unsigned int milliseconds);

        virtual void AddEntity(IEntityPtr entity);
        virtual void AddDrawable(IDrawablePtr drawable);
        virtual void AddUpdatable(IUpdatablePtr updatable);
        
        virtual ICameraPtr Camera();

    private:

        ICameraPtr mCamera;
        IDrawableCollection mDrawables;
        IUpdatableCollection mUpdatables;
    };

}

