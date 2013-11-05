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
#include "Quad.h"

using namespace mono;

EntityBase::EntityBase()
    : mRotation(0.0f),
      mScale(1.0f, 1.0f)
{ }

void EntityBase::doDraw(IRenderer& renderer) const
{
    const math::Vector2f rotationPoint = mBasePoint * mScale;
    
    // Im not so happy with this, two translations...
    
    glTranslatef(mPosition.mX + rotationPoint.mX, mPosition.mY + rotationPoint.mY, 0.0f);
    glRotatef(mRotation, 0.0f, 0.0f, 1.0f);    
    glTranslatef(-rotationPoint.mX, -rotationPoint.mY, 0.0f);    
    glScalef(mScale.mX, mScale.mY, 1.0f);

    for(auto it = mChildren.begin(), end = mChildren.end(); it != end; ++it)
    {
        const OGL::OGLPushPopMatrix raii;
        
        const IEntityPtr child = *it;
        child->doDraw(renderer);
    }
        
    Draw(renderer);    
}

void EntityBase::doUpdate(unsigned int delta)
{
    for(auto it = mChildren.begin(), end = mChildren.end(); it != end; ++it)
    {
        const IEntityPtr child = *it;
        child->doUpdate(delta);
    }
    
    Update(delta);
}

const math::Vector2f& EntityBase::Position() const
{
    return mPosition;
}

math::Quad EntityBase::BoundingBox() const
{
    const float x = mPosition.mX - (mScale.mX / 2.0f);
    const float y = mPosition.mY - (mScale.mY / 2.0f);
    math::Quad thisbb(x, y, x + mScale.mX, y + mScale.mY);
        
    for(auto it = mChildren.begin(), end = mChildren.end(); it != end; ++it)
    {
        const IEntityPtr child = *it;
        math::Quad childbb = (child->BoundingBox() * mScale);
        childbb.mA += mPosition;
        childbb.mB += mPosition;
        thisbb |= childbb;
    }
            
    return thisbb;
}

bool EntityBase::RemoveMe() const
{
    return false;
}

void EntityBase::AddChild(IEntityPtr child)
{
    mChildren.push_back(child);
}

void EntityBase::RemoveChild(IEntityPtr child)
{
    FindAndRemove(mChildren, child);
}


