/*
 *  DrawableBase.h
 *  Monolith1
 *
 *  Created by Niblit on 2011-02-21.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once

#include "IEntity.h"
#include "MonoFwd.h"
#include "Vector2f.h"

namespace mono
{

    class EntityBase : public IEntity
    {
    public:
        
        virtual void Draw(IRenderer& renderer) const = 0;
        virtual void Update(unsigned int delta) = 0;
        virtual const Math::Vector2f& Position() const;
        virtual Math::Quad BoundingBox() const;

        void AddChild(IEntityPtr child);
        void RemoveChild(IEntityPtr child);
		
    protected:
        
        EntityBase();

        virtual void doDraw(IRenderer& renderer) const;
        virtual void doUpdate(unsigned int delta);
		
        Math::Vector2f mPosition;
        Math::Vector2f mRotationCenter;
        float mScale;
        float mRotation;
        
        IEntityCollection mChildren;
    };
    
}

