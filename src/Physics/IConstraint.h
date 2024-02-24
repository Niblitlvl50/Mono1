
#pragma once

struct cpConstraint;

namespace mono
{
    class IBody;
    
    struct ConstraintBodyPair
    {
        mono::IBody* first;
        mono::IBody* second;
    };

    class IConstraint
    {
    public:

        virtual ~IConstraint() = default;

        virtual void SetMaxForce(float force) = 0;
        virtual float GetMaxForce() const = 0;

        virtual void SetMaxBias(float bias) = 0;
        virtual float GetMaxBias() const = 0;

        virtual void SetErrorBias(float bias) = 0;
        virtual float GetErrorBias() const = 0;

        virtual float GetAppliedImpulse() const = 0;
        virtual float GetAppliedForce() const = 0;
        virtual float GetAppliedForcePercentage() const = 0;

        virtual ConstraintBodyPair GetBodies() = 0;

        //! Native chipmunk handle
        virtual cpConstraint* Handle() = 0;
    };
}
