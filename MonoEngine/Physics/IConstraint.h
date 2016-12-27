
#pragma once

#include "CMFwd.h"

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

        virtual cpConstraint* Handle() = 0;
    };
}
