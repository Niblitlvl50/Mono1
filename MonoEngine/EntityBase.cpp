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

using namespace mono;

EntityBase::EntityBase()
    : mRotation(0.0f),
      mScale(1.0f)
{ }

void EntityBase::doDraw() const
{
    glTranslatef(mPosition.mX, mPosition.mY, 0.0f);
    glRotatef(mRotation, 0.0f, 0.0f, 1.0f);
    glScalef(mScale, mScale, mScale);    
    
    for(IEntityCollection::const_iterator it = mChildren.begin(), end = mChildren.end(); it != end; ++it)
    {
        const OGL::OGLPushPopMatrix raii;
        
        const IEntityPtr child = *it;
        child->doDraw();
    }
    
    Draw();
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

void EntityBase::AddChild(IEntityPtr child)
{
    mChildren.push_back(child);
}

namespace 
{
    struct EntityFinder
    {
        EntityFinder(const IEntityPtr entity)
            : mEntity(entity)
        { }
        
        bool operator()(const IEntityPtr other) const
        {
            return mEntity == other;
        }
        
        const IEntityPtr mEntity;
    };
}

void EntityBase::RemoveChild(IEntityPtr child)
{
    EntityFinder removePredicate(child);
    IEntityCollection::iterator newEnd = std::remove_if(mChildren.begin(), mChildren.end(), removePredicate);
    mChildren.erase(newEnd, mChildren.end());
}


