//
//  PhysicsEntityBase.h
//  Mono1
//
//  Created by Niblit on 2013-03-31.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#pragma once

#include "IPhysicsEntity.h"
#include "Vector2f.h"
#include "CMObject.h"

namespace mono
{
    class PhysicsEntityBase : public IPhysicsEntity
    {
    public:
        
        virtual const math::Vector2f& Position() const;
        virtual float Rotation() const;
        virtual void SetPosition(const math::Vector2f& position);
        virtual math::Quad BoundingBox() const;
        virtual cm::Object& GetPhysics();
        virtual bool RemoveMe() const;
        
    protected:
        
        PhysicsEntityBase();
        
        virtual void doDraw(mono::IRenderer& renderer) const;
        virtual void doUpdate(unsigned int delta);
        
        virtual void Draw(mono::IRenderer& renderer) const = 0;
        virtual void Update(unsigned int delta) = 0;
        
        math::Vector2f mPosition;
        math::Vector2f mScale;
        float mRotation;
        
        cm::Object mPhysicsObject;
    };
}

