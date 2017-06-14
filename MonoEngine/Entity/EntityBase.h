
#pragma once

#include "IEntity.h"
#include "MonoPtrFwd.h"
#include "Math/Vector.h"
#include "Rendering/RenderFwd.h"
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
        virtual unsigned int Id() const;
        virtual void SetProperty(unsigned int property);
        virtual bool HasProperty(unsigned int property) const;

        void AddChild(const IEntityPtr& child);
        void RemoveChild(const IEntityPtr& child);

    protected:

        EntityBase();

        virtual void Draw(mono::IRenderer& renderer) const = 0;
        virtual void Update(unsigned int delta) = 0;

        virtual void doDraw(IRenderer& renderer) const;
        virtual void doUpdate(unsigned int delta);

        const unsigned int m_uid;
        unsigned int m_properties;

        math::Vector m_position;
        math::Vector m_base_point;
        math::Vector m_scale;
        float m_rotation;

        std::vector<IEntityPtr> m_children;
    };
}
