
#pragma once

#include "CMFwd.h"
#include <vector>

namespace mono
{
    struct Object
    {
        IBodyPtr body;
        std::vector<IShapePtr> shapes;
    };
}
