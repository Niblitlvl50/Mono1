/*
 *  DrawableBase.cpp
 *  Monolith1
 *
 *  Created by Niblit on 2011-02-21.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "EntityBase.h"
#include "SysOpenGL.h"
#include "Utils.h"

using namespace mono;

EntityBase::EntityBase()
    : mRotation(0.0f),
      mScale(1.0f)
{ }

void EntityBase::doDraw(IRenderer& renderer) const
{
    const Math::Vector2f rotationPoint = mRotationCenter * mScale;
    
    // Im not so happy with this, two translations...
    
    glTranslatef(mPosition.mX + rotationPoint.mX, mPosition.mY + rotationPoint.mY, 0.0f);
    glRotatef(mRotation, 0.0f, 0.0f, 1.0f);    
    glTranslatef(-rotationPoint.mX, -rotationPoint.mY, 0.0f);
    glScalef(mScale, mScale, mScale);    
    
    for(IEntityCollection::const_iterator it = mChildren.begin(), end = mChildren.end(); it != end; ++it)
    {
        const OGL::OGLPushPopMatrix raii;
        
        const IEntityPtr child = *it;
        child->doDraw(renderer);
    }
    
    Draw(renderer);
}

void EntityBase::doUpdate(unsigned int delta)
{
    for(IEntityCollection::const_iterator it = mChildren.begin(), end = mChildren.end(); it != end; ++it)
    {
        const IEntityPtr child = *it;
        child->doUpdate(delta);
    }
    
    Update(delta);
}

const Math::Vector2f& EntityBase::Position() const
{
    return mPosition;
}

void EntityBase::AddChild(IEntityPtr child)
{
    mChildren.push_back(child);
}

void EntityBase::RemoveChild(IEntityPtr child)
{
    const bool result = FindAndRemoveEntity(mChildren, child);
}


