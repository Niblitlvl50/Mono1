
#pragma once

#include "PhysicsFwd.h"

#include <memory>
#include <functional>

namespace mono
{
    struct PhysicsData;

    using IBodyPtr = std::shared_ptr<IBody>;
    using IShapePtr = std::shared_ptr<IShape>;
    using IConstraintPtr = std::shared_ptr<IConstraint>;

    using BodyFunc = std::function<void (IBody*)>;
}
