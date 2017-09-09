
#pragma once

#include "IPhysicsEntity.h"
#include "MonoPtrFwd.h"
#include "Math/Vector.h"
#include "Physics/CMPhysicsData.h"

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
        virtual void SetBasePoint(const math::Vector& base_point);
        virtual void SetScale(const math::Vector& scale);
        virtual math::Quad BoundingBox() const;
        virtual math::Matrix Transformation() const;
        virtual mono::PhysicsData& GetPhysics();
        virtual unsigned int Id() const;
        virtual void SetProperty(unsigned int property);
        virtual bool HasProperty(unsigned int property) const;

        void AddChild(const IEntityPtr& child);
        void RemoveChild(const IEntityPtr& child);

    protected:

        PhysicsEntityBase();

        virtual void doDraw(mono::IRenderer& renderer) const;
        virtual void doUpdate(unsigned int delta);

        virtual void Draw(mono::IRenderer& renderer) const = 0;
        virtual void Update(unsigned int delta) = 0;

        const unsigned int m_uid;
        unsigned int m_properties;

        math::Vector m_position;
        math::Vector m_scale;
        float m_rotation;

        mono::PhysicsData m_physics;
        std::vector<IEntityPtr> m_children;
    };
}
