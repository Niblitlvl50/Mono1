
#pragma once

#include "CMFwd.h"
#include <vector>

namespace mono
{
    struct PhysicsData
    {
        IBodyPtr body;
        std::vector<IShapePtr> shapes;
    };
}
