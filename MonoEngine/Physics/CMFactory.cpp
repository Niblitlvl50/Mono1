
#include "CMFactory.h"
#include "Impl/BodyImpl.h"
#include "Impl/ShapeImpl.h"
#include "Impl/ConstraintImpl.h"

#include "chipmunk/chipmunk.h"

mono::IBodyPtr mono::PhysicsFactory::CreateStaticBody()
{
    return std::make_shared<cm::BodyImpl>(cpBodyNewStatic());
}

mono::IBodyPtr mono::PhysicsFactory::CreateKinematicBody()
{
    return std::make_shared<cm::BodyImpl>(cpBodyNewKinematic());
}

mono::IBodyPtr mono::PhysicsFactory::CreateBody(float mass, float inertia)
{
    return std::make_shared<cm::BodyImpl>(cpBodyNew(mass, inertia));
}

mono::IShapePtr mono::PhysicsFactory::CreateShape(mono::IBodyPtr& body, float radius, const math::Vector& offset)
{
    return std::make_shared<cm::ShapeImpl>(body, radius, offset);
}

mono::IShapePtr mono::PhysicsFactory::CreateShape(mono::IBodyPtr& body, float width, float height)
{
    return std::make_shared<cm::ShapeImpl>(body, width, height);
}

mono::IShapePtr mono::PhysicsFactory::CreateShape(mono::IBodyPtr& body, const math::Vector& first, const math::Vector& second, float radius)
{
    return std::make_shared<cm::ShapeImpl>(body, first, second, radius);
}

mono::IShapePtr mono::PhysicsFactory::CreateShape(mono::IBodyPtr& body, const std::vector<math::Vector>& vertices, const math::Vector& offset)
{
    return std::make_shared<cm::ShapeImpl>(body, vertices, offset);
}

mono::IConstraintPtr mono::PhysicsFactory::CreatePivot(const mono::IBodyPtr& a, const mono::IBodyPtr& b)
{
    cpConstraint* constraint = cpPivotJointNew(a->Handle(), b->Handle(), cpvzero);
    return std::make_shared<cm::ConstraintImpl>(constraint);
}

mono::IConstraintPtr mono::PhysicsFactory::CreateGear(const mono::IBodyPtr& a, const mono::IBodyPtr& b, float phase, float ratio)
{
    cpConstraint* constraint = cpGearJointNew(a->Handle(), b->Handle(), phase, ratio);
    return std::make_shared<cm::ConstraintImpl>(constraint);
}

mono::IConstraintPtr mono::PhysicsFactory::CreateSpring(const mono::IBodyPtr& a, const mono::IBodyPtr& b, float restLength, float stiffness, float damping)
{
    cpConstraint* constraint = cpDampedSpringNew(a->Handle(), b->Handle(), cpvzero, cpvzero, restLength, stiffness, damping);
    return std::make_shared<cm::ConstraintImpl>(constraint);
}