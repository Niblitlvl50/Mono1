
#pragma once

struct cpConstraint;

namespace mono
{
    class IConstraint
    {
    public:

        virtual ~IConstraint()
        { }

        virtual void SetMaxForce(float force) = 0;
        virtual float GetMaxForce() const = 0;

        virtual void SetMaxBias(float bias) = 0;
        virtual float GetMaxBias() const = 0;

        virtual void SetErrorBias(float bias) = 0;
        virtual float GetErrorBias() const = 0;

        //! Native chipmunk handle
        virtual cpConstraint* Handle() = 0;
    };
}
