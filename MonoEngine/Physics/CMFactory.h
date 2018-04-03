
#pragma once

#include "CMFwd.h"
#include "Math/MathFwd.h"
#include <vector>

namespace mono
{
    class PhysicsFactory
    {
    public:

        static IBodyPtr CreateStaticBody();
        static IBodyPtr CreateKinematicBody();

        //! Inertia is the body's resistance to change in rotation, pass in INFINITY
        //! if no rotation is wanted.
        static IBodyPtr CreateBody(float mass, float inertia);

        //! Circle shape
        static IShapePtr CreateShape(IBodyPtr& body, float radius, const math::Vector& offset);

        //! Box shape
        static IShapePtr CreateShape(IBodyPtr& body, float width, float height);

        //! Segment shape
        static IShapePtr CreateShape(IBodyPtr& body, const math::Vector& first, const math::Vector& second, float radius);

        //! Polygon shape
        static IShapePtr CreateShape(IBodyPtr& body, const std::vector<math::Vector>& vertices, const math::Vector& offset);

        //! Constraints
        static mono::IConstraintPtr CreatePivot(const mono::IBodyPtr& a, const mono::IBodyPtr& b);
        static mono::IConstraintPtr CreateGear(const mono::IBodyPtr& a, const mono::IBodyPtr& b, float phase, float ratio);
        static mono::IConstraintPtr CreateSpring(const mono::IBodyPtr& a, const mono::IBodyPtr& b, float restLength, float stiffness, float damping);
    };
}
