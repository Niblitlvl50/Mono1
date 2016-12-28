
#pragma once

#include "CMFwd.h"

namespace mono
{
    class ConstraintsFactory
    {
    public:
        static mono::IConstraintPtr CreatePivot(const mono::IBodyPtr& a, const mono::IBodyPtr& b);
        static mono::IConstraintPtr CreateGear(const mono::IBodyPtr& a, const mono::IBodyPtr& b, float phase, float ratio);
        static mono::IConstraintPtr CreateSpring(const mono::IBodyPtr& a, const mono::IBodyPtr& b, float restLength, float stiffness, float damping);
    };
}
