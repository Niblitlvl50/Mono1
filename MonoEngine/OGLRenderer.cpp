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
#include "ITexture.h"

using namespace mono;

void OGLRenderer::DrawFrame() const
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();
	glTranslatef(0.0f, 0.0f, -10.0f);
    
    for(IDrawableCollection::const_iterator it = mDrawables.begin(), end = mDrawables.end(); it != end; ++it)
    {
        IDrawablePtr drawable = *it;
        drawable->Draw();
    }
}

void OGLRenderer::Update(unsigned int milliseconds)
{
    for(IUpdatableCollection::iterator it = mUpdatables.begin(), end = mUpdatables.end(); it != end; ++it)
    {
        IUpdatablePtr updatable = *it;
        updatable->Update(milliseconds);
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



