/*
 *  OGLRenderer.cpp
 *  Monolith1
 *
 *  Created by Niblit on 2011-02-08.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "OGLRenderer.h"
#include <GLUT/glut.h>

#include "IEntity.h"

using namespace mono;


void OGLRenderer::DrawFrame() const
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();
	glTranslatef(0.0f, 0.0f, -10.0f);
    
    for(IEntityCollection::const_iterator it = mEntities.begin(), end = mEntities.end(); it != end; ++it)
    {
        IEntityPtr drawable = *it;
        drawable->Draw();
    }
}

void OGLRenderer::Update(unsigned int milliseconds)
{
    for(IEntityCollection::iterator it = mEntities.begin(), end = mEntities.end(); it != end; ++it)
    {
        IEntityPtr updatable = *it;
        updatable->Update(milliseconds);
    }
}

void OGLRenderer::AddEntity(IEntityPtr entity)
{
    mEntities.push_back(entity);
}

