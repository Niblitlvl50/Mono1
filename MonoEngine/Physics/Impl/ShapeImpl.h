
#pragma once

#include "Physics/IShape.h"
#include "Math/MathFwd.h"

#include <vector>

namespace cm
{
    class ShapeImpl : public mono::IShape
    {
    public:
     
        ShapeImpl(mono::IBodyPtr& body, float radius, const math::Vector& offset);
        ShapeImpl(mono::IBodyPtr& body, float width, float height);
        ShapeImpl(mono::IBodyPtr& body, const math::Vector& first, const math::Vector& second, float radius);
        ShapeImpl(mono::IBodyPtr& body, const std::vector<math::Vector>& vertices, const math::Vector& offset);
        ~ShapeImpl();

        void SetElasticity(float value) override;
        void SetFriction(float value) override;
        float GetInertiaValue() const override;
        void SetCollisionFilter(unsigned int category, unsigned int mask) override;
        cpShape* Handle() override;

    private:
        
        cpShape* m_shape;
        float m_inertia_value;
    };    
}
