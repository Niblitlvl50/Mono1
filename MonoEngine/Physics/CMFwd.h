
#pragma once

#include <memory>
#include <functional>

struct cpBody;
struct cpShape;
struct cpConstraint;

namespace mono
{
    struct PhysicsData;
    struct IBody;
    struct IShape;
    class IConstraint;

    using IBodyPtr = std::shared_ptr<IBody>;
    using IShapePtr = std::shared_ptr<IShape>;
    using IConstraintPtr = std::shared_ptr<IConstraint>;
    using BodyFunc = std::function<void (const IBodyPtr&)>;
}
