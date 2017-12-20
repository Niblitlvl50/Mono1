
#include "CMFactory.h"
#include "IBody.h"
#include "IShape.h"
#include "Math/Vector.h"
#include "Math/MathFunctions.h"

#include "chipmunk/chipmunk.h"

#include <algorithm>

namespace
{
    struct CMBody : mono::IBody
    {
        CMBody(cpBody* body)
            : mBody(body),
              mHandler(nullptr)
        { }
        ~CMBody()
        {
            cpBodyFree(mBody);
        }
        bool IsStatic() const override
        {
            return cpBodyGetType(mBody) == CP_BODY_TYPE_STATIC;
        }
        void SetMass(float mass) override
        {
            cpBodySetMass(mBody, mass);
        }
        float GetMass() const override
        {
            return cpBodyGetMass(mBody);
        }
        void SetAngle(float angle) override
        {
            cpBodySetAngle(mBody, angle);
        }
        float GetAngle() const override
        {
            return cpBodyGetAngle(mBody);
        }
        void SetMoment(float moment) override
        {
            cpBodySetMoment(mBody, moment);
        }
        float GetMoment() const override
        {
            return cpBodyGetMoment(mBody);
        }
        void SetPosition(const math::Vector& position) override
        {
            cpBodySetPosition(mBody, cpv(position.x, position.y));
        }
        math::Vector GetPosition() const override
        {
            const cpVect& position = cpBodyGetPosition(mBody);
            return math::Vector(position.x, position.y);
        }
        void ApplyForce(const math::Vector& force, const math::Vector& offset) override
        {
            cpBodyApplyForceAtWorldPoint(mBody, cpv(force.y, force.y), cpv(offset.x, offset.y));
        }
        void ApplyLocalForce(const math::Vector& force, const math::Vector& offset) override
        {
            cpBodyApplyForceAtLocalPoint(mBody, cpv(force.y, force.y), cpv(offset.x, offset.y));
        }
        void ApplyImpulse(const math::Vector& impulse, const math::Vector& offset) override
        {
            cpBodyApplyImpulseAtWorldPoint(mBody, cpv(impulse.x, impulse.y), cpv(offset.x, offset.y));
        }
        void ApplyLocalImpulse(const math::Vector& impulse, const math::Vector& offset) override
        {
            cpBodyApplyImpulseAtLocalPoint(mBody, cpv(impulse.x, impulse.y), cpv(offset.x, offset.y));
        }
        void SetVelocity(const math::Vector& velocity) override
        {
            cpBodySetVelocity(mBody, cpv(velocity.x, velocity.y));
        }
        void ResetForces() override
        {
            cpBodySetForce(mBody, cpvzero);
        }
        void SetCollisionHandler(mono::ICollisionHandler* handler) override
        {
            mHandler = handler;
        }
        void OnCollideWith(const mono::IBodyPtr& body, unsigned int categories) override
        {
            if(mHandler)
                mHandler->OnCollideWith(body, categories);
        }
        cpBody* Handle() override
        {
            return mBody;
        }
        
        cpBody* mBody;
        mono::ICollisionHandler* mHandler;
    };
    
    struct CMShape : mono::IShape
    {
        CMShape(mono::IBodyPtr& body, float radius, const math::Vector& offset)
        {
            mShape = cpCircleShapeNew(body->Handle(), radius, cpv(offset.x, offset.y));
            mInertiaValue = cpMomentForCircle(body->GetMass(), 0.0f, radius, cpv(offset.x, offset.y));
        }
        CMShape(mono::IBodyPtr& body, float width, float height)
        {
            mShape = cpBoxShapeNew(body->Handle(), width, height, 1.0f);
            mInertiaValue = cpMomentForBox(body->GetMass(), width, height);
        }
        CMShape(mono::IBodyPtr& body, const math::Vector& first, const math::Vector& second, float radius)
        {
            mShape = cpSegmentShapeNew(body->Handle(), cpv(first.x, first.y), cpv(second.x, second.y), radius);
            mInertiaValue = cpMomentForSegment(body->GetMass(), cpv(first.x, first.y), cpv(second.x, second.y), radius);
        }
        CMShape(mono::IBodyPtr& body, const std::vector<math::Vector>& vertices, const math::Vector& offset)
        {
            const auto transformFunc = [](const math::Vector& position) {
                return cpv(position.x, position.y);
            };
            
            std::vector<cpVect> vects(vertices.size());
            std::transform(vertices.begin(), vertices.end(), vects.begin(), transformFunc);

            const bool clockwice = math::IsPolygonClockwise(vertices);
            if(clockwice)
                std::reverse(vects.begin(), vects.end());

            mShape = cpPolyShapeNewRaw(body->Handle(), int(vects.size()), vects.data(), 0.1f);
            mInertiaValue = cpMomentForPoly(body->GetMass(), int(vects.size()), vects.data(), cpv(offset.x, offset.y), 1.0f);
        }
        ~CMShape()
        {
            cpShapeFree(mShape);
        }
        void SetElasticity(float value) override
        {
            cpShapeSetElasticity(mShape, value);
        }
        void SetFriction(float value) override
        {
            cpShapeSetFriction(mShape, value);
        }
        float GetInertiaValue() const override
        {
            return mInertiaValue;
        }
        void SetCollisionFilter(unsigned int category, unsigned int mask) override
        {
            cpShapeFilter filter = cpShapeGetFilter(mShape);
            filter.categories = category;
            filter.mask = mask;
            cpShapeSetFilter(mShape, filter);
        }
        cpShape* Handle() override
        {
            return mShape;
        }
        
        cpShape* mShape;
        float mInertiaValue;
    };
}

mono::IBodyPtr mono::PhysicsFactory::CreateStaticBody()
{
    return std::make_shared<CMBody>(cpBodyNewStatic());
}

mono::IBodyPtr mono::PhysicsFactory::CreateKinematicBody()
{
    return std::make_shared<CMBody>(cpBodyNewKinematic());
}

mono::IBodyPtr mono::PhysicsFactory::CreateBody(float mass, float inertia)
{
    return std::make_shared<CMBody>(cpBodyNew(mass, inertia));
}

mono::IShapePtr mono::PhysicsFactory::CreateShape(mono::IBodyPtr& body, float radius, const math::Vector& offset)
{
    return std::make_shared<CMShape>(body, radius, offset);
}

mono::IShapePtr mono::PhysicsFactory::CreateShape(mono::IBodyPtr& body, float width, float height)
{
    return std::make_shared<CMShape>(body, width, height);
}

mono::IShapePtr mono::PhysicsFactory::CreateShape(mono::IBodyPtr& body, const math::Vector& first, const math::Vector& second, float radius)
{
    return std::make_shared<CMShape>(body, first, second, radius);
}

mono::IShapePtr mono::PhysicsFactory::CreateShape(mono::IBodyPtr& body, const std::vector<math::Vector>& vertices, const math::Vector& offset)
{
    return std::make_shared<CMShape>(body, vertices, offset);
}
