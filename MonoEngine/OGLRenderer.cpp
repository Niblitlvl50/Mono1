/*
 *  OGLRenderer.cpp
 *  Monolith1
 *
 *  Created by Niblit on 2011-02-08.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "OGLRenderer.h"
#include "SysOpenGL.h"
#include "IEntity.h"

using namespace mono;

OGLRenderer::OGLRenderer(ICameraPtr camera)
    : mCamera(camera)
{ }

void OGLRenderer::DrawFrame() const
{    
    for(IDrawableCollection::const_iterator it = mDrawables.begin(), end = mDrawables.end(); it != end; ++it)
    {
        const OGL::OGLPushPopMatrix raii;

        const IDrawablePtr drawable = *it;
        drawable->doDraw();
    }
}

void OGLRenderer::Update(unsigned int milliseconds)
{
    for(IUpdatableCollection::iterator it = mUpdatables.begin(), end = mUpdatables.end(); it != end; ++it)
    {
        IUpdatablePtr updatable = *it;
        updatable->doUpdate(milliseconds);
    }
}

void OGLRenderer::AddEntity(IEntityPtr entity)
{
    mDrawables.push_back(entity);
    mUpdatables.push_back(entity);
}
void OGLRenderer::AddDrawable(IDrawablePtr drawable)
{
    mDrawables.push_back(drawable);
}
void OGLRenderer::AddUpdatable(IUpdatablePtr updatable)
{
    mUpdatables.push_back(updatable);
}



