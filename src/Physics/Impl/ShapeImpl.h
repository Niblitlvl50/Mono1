
#pragma once

#include "Physics/IShape.h"

namespace cm
{
    class ShapeImpl : public mono::IShape
    {
    public:
     
        ShapeImpl();
        ShapeImpl(cpShape* shape, float inertia_value);

        void SetShapeHandle(cpShape* shape);
        
        void SetElasticity(float value) override;
        void SetFriction(float value) override;
        void SetInertia(float inertia);
        float GetInertiaValue() const override;
        void SetCollisionFilter(uint32_t category, uint32_t mask) override;
        cpShape* Handle() override;

    private:
        
        cpShape* m_shape;
        float m_inertia_value;
    };    
}
