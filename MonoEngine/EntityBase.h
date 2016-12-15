
#pragma once

#include "IEntity.h"
#include "MonoPtrFwd.h"
#include "Math/Vector2f.h"
#include <vector>

namespace mono
{
    class EntityBase : public IEntity
    {
    public:
    
        virtual const math::Vector& Position() const;
        virtual const math::Vector& BasePoint() const;
        virtual float Rotation() const;
        virtual const math::Vector& Scale() const;

        virtual void SetPosition(const math::Vector& position);
        virtual void SetBasePoint(const math::Vector& base_point);
        virtual void SetRotation(float rotation);
        virtual void SetScale(const math::Vector& scale);

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

        math::Vector mPosition;
        math::Vector mBasePoint;
        math::Vector mScale;
        float mRotation;

        std::vector<IEntityPtr> mChildren;
    };
}
