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

namespace mono
{

    class EntityBase : public IEntity
    {
    public:

        virtual void Draw() const;
        virtual void Update(unsigned int delta);

        void AddChild(IEntityPtr child);
        void RemoveChild(IEntityPtr child);
		
    protected:

        EntityBase();
		
        IEntityCollection mChildren;
    };
    
}

