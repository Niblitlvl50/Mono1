
#pragma once

#include <memory>

struct cpBody;
struct cpShape;
struct cpConstraint;

namespace mono
{
    struct PhysicsData;
    class IBody;
    class IShape;
    class IConstraint;

    using IBodyPtr = std::shared_ptr<IBody>;
    using IShapePtr = std::shared_ptr<IShape>;
    using IConstraintPtr = std::shared_ptr<IConstraint>;
}
