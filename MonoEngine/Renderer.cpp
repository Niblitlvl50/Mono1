/*
 *  OGLRenderer.cpp
 *  Monolith1
 *
 *  Created by Niblit on 2011-02-08.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "Renderer.h"
#include "SysOpenGL.h"
#include "IEntity.h"
#include "ICamera.h"

using namespace mono;

Renderer::Renderer(ICameraPtr camera)
    : mCamera(camera)
{ }

void Renderer::DrawFrame() const
{    
    for(IDrawableCollection::const_iterator it = mDrawables.begin(), end = mDrawables.end(); it != end; ++it)
    {
        const OGL::OGLPushPopMatrix raii;

        const IDrawablePtr drawable = *it;
        drawable->doDraw();
    }
}

void Renderer::Update(unsigned int milliseconds)
{
    mCamera->Update(milliseconds);
    
    for(IUpdatableCollection::iterator it = mUpdatables.begin(), end = mUpdatables.end(); it != end; ++it)
    {
        IUpdatablePtr updatable = *it;
        updatable->doUpdate(milliseconds);
    }
}

void Renderer::AddEntity(IEntityPtr entity)
{
    mDrawables.push_back(entity);
    mUpdatables.push_back(entity);
}
void Renderer::AddDrawable(IDrawablePtr drawable)
{
    mDrawables.push_back(drawable);
}
void Renderer::AddUpdatable(IUpdatablePtr updatable)
{
    mUpdatables.push_back(updatable);
}

ICameraPtr Renderer::Camera()
{
    return mCamera;
}


