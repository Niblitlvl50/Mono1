
#pragma once

#include "IPhysicsEntity.h"
#include "MonoPtrFwd.h"
#include "Math/Vector.h"
#include "Physics/CMObject.h"

namespace mono
{
    class PhysicsEntityBase : public IPhysicsEntity
    {
    public:

        virtual const math::Vector& Position() const;
        virtual const math::Vector& BasePoint() const;
        virtual float Rotation() const;
        virtual const math::Vector& Scale() const;
        virtual void SetPosition(const math::Vector& position);
        virtual void SetRotation(float rotation);
        virtual void SetScale(const math::Vector& scale);
        virtual math::Quad BoundingBox() const;
        virtual math::Matrix Transformation() const;
        virtual mono::PhysicsData& GetPhysics();
        virtual uint Id() const;
        virtual void SetProperty(uint property);
        virtual bool HasProperty(uint property) const;

        void AddChild(const IEntityPtr& child);
        void RemoveChild(const IEntityPtr& child);

    protected:

        PhysicsEntityBase();

        virtual void doDraw(mono::IRenderer& renderer) const;
        virtual void doUpdate(unsigned int delta);

        virtual void Draw(mono::IRenderer& renderer) const = 0;
        virtual void Update(unsigned int delta) = 0;

        const uint m_uid;
        uint m_properties;

        math::Vector mPosition;
        math::Vector mScale;
        float mRotation;

        mono::PhysicsData mPhysicsObject;
        std::vector<IEntityPtr> m_children;
    };
}
