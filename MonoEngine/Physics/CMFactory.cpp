//
//  CMShapeFactory.cpp
//  Mono1
//
//  Created by Niblit on 2012-08-23.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "CMFactory.h"
#include "CMIBody.h"
#include "CMIShape.h"
#include "Vector2f.h"
#include "MathFunctions.h"

#include "chipmunk.h"

namespace
{
    struct CMBody : mono::IBody
    {        
        CMBody()
            : mHandler(nullptr)
        {
            mBody = cpBodyNewStatic();
        }
        CMBody(float mass, float inertia)
            : mHandler(nullptr)
        {
            mBody = cpBodyNew(mass, inertia);
        }
        ~CMBody()
        {
            cpBodyFree(mBody);
        }
        bool IsStatic() const
        {
            return cpBodyGetType(mBody) == CP_BODY_TYPE_STATIC;
        }
        void SetMass(float mass)
        {
            cpBodySetMass(mBody, mass);
        }
        float GetMass() const
        {
            return cpBodyGetMass(mBody);
        }
        void SetAngle(float angle)
        {
            cpBodySetAngle(mBody, angle);
        }
        float GetAngle() const
        {
            return cpBodyGetAngle(mBody);
        }
        void SetMoment(float moment)
        {
            cpBodySetMoment(mBody, moment);
        }
        float GetMoment() const
        {
            return cpBodyGetMoment(mBody);
        }
        void SetPosition(const math::Vector2f& position)
        {
            cpBodySetPosition(mBody, cpv(position.x, position.y));
        }
        math::Vector2f GetPosition() const
        {
            const cpVect& position = cpBodyGetPosition(mBody);
            return math::Vector2f(position.x, position.y);
        }
        void ApplyForce(const math::Vector2f& force, const math::Vector2f& offset)
        {
            cpBodyApplyForceAtWorldPoint(mBody, cpv(force.y, force.y), cpv(offset.x, offset.y));
        }
        void ApplyLocalForce(const math::Vector2f& force, const math::Vector2f& offset)
        {
            cpBodyApplyForceAtLocalPoint(mBody, cpv(force.y, force.y), cpv(offset.x, offset.y));
        }
        void ApplyImpulse(const math::Vector2f& impulse, const math::Vector2f& offset)
        {
            cpBodyApplyImpulseAtWorldPoint(mBody, cpv(impulse.x, impulse.y), cpv(offset.x, offset.y));
        }
        virtual void ApplyLocalImpulse(const math::Vector2f& impulse, const math::Vector2f& offset)
        {
            cpBodyApplyImpulseAtLocalPoint(mBody, cpv(impulse.x, impulse.y), cpv(offset.x, offset.y));
        }
        void SetVelocity(const math::Vector2f& velocity)
        {
            cpBodySetVelocity(mBody, cpv(velocity.x, velocity.y));
        }
        void ResetForces()
        {
            cpBodySetForce(mBody, cpvzero);
        }
        void SetCollisionHandler(mono::ICollisionHandler* handler)
        {
            mHandler = handler;
        }
        void OnCollideWith(mono::IBodyPtr body)
        {
            if(mHandler)
                mHandler->OnCollideWith(body);
        }
        void OnPostStep()
        {
            if(mHandler)
                mHandler->OnPostStep();
        }
        cpBody* Body()
        {
            return mBody;
        }
        
        mono::ICollisionHandler* mHandler;
        cpBody* mBody;
    };
    
    struct CMShape : mono::IShape
    {
        CMShape(mono::IBodyPtr body, float radius, const math::Vector2f& offset)
        {
            mShape = cpCircleShapeNew(body->Body(), radius, cpv(offset.x, offset.y));
            mInertiaValue = cpMomentForCircle(body->GetMass(), 0.0f, radius, cpv(offset.x, offset.y));
        }
        CMShape(mono::IBodyPtr body, float width, float height)
        {
            mShape = cpBoxShapeNew(body->Body(), width, height, 1.0f);
            mInertiaValue = cpMomentForBox(body->GetMass(), width, height);
        }
        CMShape(mono::IBodyPtr body, const math::Vector2f& first, const math::Vector2f& second, float radius)
        {
            mShape = cpSegmentShapeNew(body->Body(), cpv(first.x, first.y), cpv(second.x, second.y), radius);
            mInertiaValue = cpMomentForSegment(body->GetMass(), cpv(first.x, first.y), cpv(second.x, second.y), radius);
        }
        CMShape(mono::IBodyPtr body, const std::vector<math::Vector2f>& vertices, const math::Vector2f& offset)
        {
            const auto transformFunc = [](const math::Vector2f& position) {
                return cpv(position.x, position.y);
            };
            
            std::vector<cpVect> vects(vertices.size());
            std::transform(vertices.begin(), vertices.end(), vects.begin(), transformFunc);
            
            mShape = cpPolyShapeNewRaw(body->Body(), int(vects.size()), vects.data(), 1.0f);
            mInertiaValue = cpMomentForPoly(body->GetMass(), int(vects.size()), vects.data(), cpv(offset.x, offset.y), 1.0f);
        }
        ~CMShape()
        {
            cpShapeFree(mShape);
        }
        void SetElasticity(float value)
        {
            cpShapeSetElasticity(mShape, value);
        }
        void SetFriction(float value)
        {
            cpShapeSetFriction(mShape, value);
        }
        float GetInertiaValue() const
        {
            return mInertiaValue;
        }
        cpShape* Shape()
        {
            return mShape;
        }
        
        cpShape* mShape;
        float mInertiaValue;
    };
}

mono::IBodyPtr mono::PhysicsFactory::CreateStaticBody()
{
    return std::make_shared<CMBody>();
}

mono::IBodyPtr mono::PhysicsFactory::CreateBody(float mass, float inertia)
{
    return std::make_shared<CMBody>(mass, inertia);
}

mono::IShapePtr mono::PhysicsFactory::CreateShape(mono::IBodyPtr body, float radius, const math::Vector2f& offset)
{
    return std::make_shared<CMShape>(body, radius, offset);
}

mono::IShapePtr mono::PhysicsFactory::CreateShape(mono::IBodyPtr body, float width, float height)
{
    return std::make_shared<CMShape>(body, width, height);
}

mono::IShapePtr mono::PhysicsFactory::CreateShape(mono::IBodyPtr body, const math::Vector2f& first, const math::Vector2f& second, float radius)
{
    return std::make_shared<CMShape>(body, first, second, radius);
}

mono::IShapePtr mono::PhysicsFactory::CreateShape(mono::IBodyPtr body, const std::vector<math::Vector2f>& vertices, const math::Vector2f& offset)
{
    return std::make_shared<CMShape>(body, vertices, offset);
}
