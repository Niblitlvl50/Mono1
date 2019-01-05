
#pragma once

#include "Physics/IShape.h"

namespace cm
{
    class ShapeImpl : public mono::IShape
    {
    public:
     
        ShapeImpl(cpShape* shape, float inertia_value);
        
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