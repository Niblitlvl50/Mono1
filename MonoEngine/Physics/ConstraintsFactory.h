
#pragma once

#include "CMFwd.h"

namespace mono
{
    class ConstraintsFactory
    {
    public:
        static mono::IConstraintPtr CreatePivot(const mono::IBodyPtr& a, const mono::IBodyPtr& b);
    };
}
