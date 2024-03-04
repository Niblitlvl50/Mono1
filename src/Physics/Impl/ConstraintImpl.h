
#pragma once

#include "Physics/PhysicsFwd.h"
#include "Physics/IConstraint.h"

namespace cm
{
    class ConstraintImpl : public mono::IConstraint
    {
    public:

        ConstraintImpl();

        void Init(mono::PhysicsSystem* physics_system);
        void SetHandle(cpConstraint* constraint);

        void SetMaxForce(float force) override;
        float GetMaxForce() const override;
        void SetMaxBias(float bias) override;
        float GetMaxBias() const override;
        void SetErrorBias(float bias) override;
        float GetErrorBias() const override;
        float GetAppliedImpulse() const override;
        float GetAppliedForce() const override;
        float GetAppliedForcePercentage() const override;
        mono::ConstraintBodyPair GetBodies() override;
        cpConstraint* Handle() override;

    private:

        mono::PhysicsSystem* m_physics_system;
        cpConstraint* m_constraint;
    };    
}
