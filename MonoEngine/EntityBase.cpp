/*
 *  DrawableBase.cpp
 *  Monolith1
 *
 *  Created by Niblit on 2011-02-21.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "EntityBase.h"

using namespace mono;

EntityBase::EntityBase()
{ }

void EntityBase::Draw() const
{
    for(IEntityCollection::const_iterator it = mChildren.begin(), end = mChildren.end(); it != end; ++it)
    {
        const IEntityPtr child = *it;
        child->Draw();
    }
}

void EntityBase::Update(unsigned int delta)
{
    for(IEntityCollection::const_iterator it = mChildren.begin(), end = mChildren.end(); it != end; ++it)
    {
        const IEntityPtr child = *it;
        child->Update(delta);
    }
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


