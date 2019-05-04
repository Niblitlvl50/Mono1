
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
        
        virtual uint32_t Id() const;
        virtual void SetId(uint32_t new_id);

        virtual void SetProperty(uint32_t property);
        virtual bool HasProperty(uint32_t property) const;

        void AddChild(const IEntityPtr& child);
        void RemoveChild(const IEntityPtr& child);

    protected:

        EntityBase();

        virtual void Draw(mono::IRenderer& renderer) const = 0;
        virtual void Update(const UpdateContext& update_context) = 0;

        virtual void doDraw(IRenderer& renderer) const;
        virtual void doUpdate(const UpdateContext& update_context);

        uint32_t m_uid;
        uint32_t m_properties;

        math::Vector m_position;
        math::Vector m_pivot_point;
        math::Vector m_scale;
        float m_rotation;

        std::vector<IEntityPtr> m_children;
    };
}
