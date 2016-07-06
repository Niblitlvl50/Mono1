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
#include "SysTypes.h"

namespace mono
{
    struct IEntity : public IDrawable, public IUpdatable
    {
        virtual ~IEntity()
        { }
        
        virtual const math::Vector2f Position() const = 0;
        virtual void SetPosition(const math::Vector2f& position) = 0;

        virtual void SetScale(const math::Vector2f& scale) = 0;

        // Rotation in radians
        virtual float Rotation() const = 0;
        virtual void SetRotation(float rotation) = 0;

        // Unique id
        virtual uint Id() const = 0;

        virtual void SetProperty(uint property) = 0;
        virtual bool HasProperty(uint property) const = 0;
    };
}

