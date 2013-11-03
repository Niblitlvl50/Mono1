//
//  IEntity.h
//  Mono1
//
//  Created by Niblit on 2012-04-06.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//


#pragma once

#include "IDrawable.h"
#include "IUpdatable.h"
#include "CMFwd.h"

namespace mono
{
    struct IEntity : public IDrawable, public IUpdatable
    {
        virtual ~IEntity()
        { }
        
        virtual const math::Vector2f& Position() const = 0;
        virtual bool RemoveMe() const = 0;
    };
    
    struct IPhysicsEntity : public IEntity
    {
        virtual ~IPhysicsEntity()
        { }
        
        virtual cm::Object& GetPhysics() = 0;
    };
}

