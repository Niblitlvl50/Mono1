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
#include "MonoPtrFwd.h"
#include "Vector2f.h"
#include <vector>

namespace mono
{

    class EntityBase : public IEntity
    {
    public:
    
        virtual const math::Vector2f& Position() const;
        virtual const math::Vector2f& BasePoint() const;
        virtual float Rotation() const;

        virtual void SetPosition(const math::Vector2f& position);
        virtual void SetBasePoint(const math::Vector2f& base_point);
        virtual void SetRotation(float rotation);
        virtual void SetScale(const math::Vector2f& scale);

        virtual math::Quad BoundingBox() const;
        virtual math::Matrix Transformation() const;
        virtual uint Id() const;
        virtual void SetProperty(uint property);
        virtual bool HasProperty(uint property) const;

        void AddChild(const IEntityPtr& child);
        void RemoveChild(const IEntityPtr& child);
    
    protected:
        
        EntityBase();

        virtual void Draw(mono::IRenderer& renderer) const = 0;
        virtual void Update(unsigned int delta) = 0;

        virtual void doDraw(IRenderer& renderer) const;
        virtual void doUpdate(unsigned int delta);

        const uint m_uid;
        uint m_properties;

        math::Vector2f mPosition;
        math::Vector2f mBasePoint;
        math::Vector2f mScale;
        float mRotation;
        
        std::vector<IEntityPtr> mChildren;
    };
    
}

