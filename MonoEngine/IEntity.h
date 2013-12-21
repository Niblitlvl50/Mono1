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

namespace mono
{
    struct IEntity : public IDrawable, public IUpdatable
    {
        virtual ~IEntity()
        { }
        
        virtual const math::Vector2f& Position() const = 0;
        virtual float Rotation() const = 0;
        virtual void SetPosition(const math::Vector2f& position) = 0;
        virtual bool RemoveMe() const = 0;
    };
}

