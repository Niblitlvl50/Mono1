
#pragma once

#include "IEntity.h"
#include "MonoFwd.h"
#include "Math/Vector.h"
#include "Rendering/RenderFwd.h"
#include <vector>

namespace mono
{
    class EntityBase : public IEntity
    {
    public:
    
        const math::Vector& Position() const override;
        const math::Vector& BasePoint() const override;
        float Rotation() const override;
        const math::Vector& Scale() const override;

        void SetPosition(const math::Vector& position) override;
        void SetBasePoint(const math::Vector& base_point) override;
        void SetRotation(float rotation) override;
        void SetScale(const math::Vector& scale) override;

        math::Quad BoundingBox() const override;
        math::Matrix Transformation() const override;
        
        uint32_t Id() const override;
        void SetId(uint32_t new_id) override;

        void SetProperty(uint32_t property) override;
        bool HasProperty(uint32_t property) const override;

        void AddChild(IEntity* child);
        void RemoveChild(IEntity* child);

    protected:

        EntityBase();
        ~EntityBase();

        virtual void EntityDraw(mono::IRenderer& renderer) const = 0;
        virtual void EntityUpdate(const mono::UpdateContext& update_context) = 0;

        void Draw(IRenderer& renderer) const override;
        void Update(const UpdateContext& update_context) override;

        uint32_t m_uid;
        uint32_t m_properties;

        math::Vector m_position;
        math::Vector m_pivot_point;
        math::Vector m_scale;
        float m_rotation;

        std::vector<mono::IEntity*> m_children;
    };
}
