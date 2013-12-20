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
#include "chipmunk.h"

namespace
{
    struct CMBody : cm::IBody
    {        
        CMBody()
            : mHandler(0)
        {
            mBody = cpBodyNewStatic();
        }
        CMBody(float mass, float inertia)
            : mHandler(0)
        {
            mBody = cpBodyNew(mass, inertia);
        }
        ~CMBody()
        {
            cpBodyFree(mBody);
        }
        bool IsStatic() const
        {
            return cpBodyIsStatic(mBody);
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
            cpBodySetPos(mBody, cpv(position.mX, position.mY));
        }
        math::Vector2f GetPosition() const
        {
            return math::Vector2f(mBody->p.x, mBody->p.y);
        }
        void ApplyForce(const math::Vector2f& force, const math::Vector2f& offset)
        {
            cpBodyApplyForce(mBody, cpv(force.mX, force.mY), cpv(offset.mX, offset.mY));
        }
        void ApplyImpulse(const math::Vector2f& impulse, const math::Vector2f& offset)
        {
            cpBodyApplyImpulse(mBody, cpv(impulse.mX, impulse.mY), cpv(offset.mX, offset.mY));
        }
        void SetVelocity(const math::Vector2f& velocity)
        {
            cpBodySetVel(mBody, cpv(velocity.mX, velocity.mY));
        }
        void ResetForces()
        {
            cpBodyResetForces(mBody);
        }
        void SetCollisionHandler(cm::ICollisionHandler* handler)
        {
            mHandler = handler;
        }
        void OnCollideWith(cm::IBodyPtr body)
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
        
        cm::ICollisionHandler* mHandler;
        cpBody* mBody;
    };
    
    struct CMShape : cm::IShape
    {
        CMShape(cm::IBodyPtr body, float radius, const math::Vector2f& offset)
        {
            mShape = cpCircleShapeNew(body->Body(), radius, cpv(offset.mX, offset.mY));
            mInertiaValue = cpMomentForCircle(body->GetMass(), 0.0f, radius, cpv(offset.mX, offset.mY));
        }
        CMShape(cm::IBodyPtr body, float width, float height)
        {
            mShape = cpBoxShapeNew(body->Body(), width, height);
            mInertiaValue = cpMomentForBox(body->GetMass(), width, height);
        }
        CMShape(cm::IBodyPtr body, const math::Vector2f& first, const math::Vector2f& second, float radius)
        {
            mShape = cpSegmentShapeNew(body->Body(), cpv(first.mX, first.mY), cpv(second.mX, second.mY), radius);
            mInertiaValue = cpMomentForSegment(body->GetMass(), cpv(first.mX, first.mY), cpv(second.mX, second.mY));
        }
        CMShape(cm::IBodyPtr body, const std::vector<math::Vector2f>& vertices, const math::Vector2f& offset)
        {
            const auto transformFunc = [](const math::Vector2f& position) {
                return cpv(position.mX, position.mY);
            };
            
            std::vector<cpVect> vects(vertices.size());
            std::transform(vertices.begin(), vertices.end(), vects.begin(), transformFunc);
            
            mShape = cpPolyShapeNew(body->Body(), int(vects.size()), vects.data(), cpv(offset.mX, offset.mY));
            mInertiaValue = cpMomentForPoly(body->GetMass(), int(vects.size()), vects.data(), cpv(offset.mX, offset.mY));
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

cm::IBodyPtr cm::Factory::CreateStaticBody()
{
    return std::make_shared<CMBody>();
}

cm::IBodyPtr cm::Factory::CreateBody(float mass, float inertia)
{
    return std::make_shared<CMBody>(mass, inertia);
}

cm::IShapePtr cm::Factory::CreateShape(cm::IBodyPtr body, float radius, const math::Vector2f& offset)
{
    return std::make_shared<CMShape>(body, radius, offset);
}

cm::IShapePtr cm::Factory::CreateShape(cm::IBodyPtr body, float width, float height)
{
    return std::make_shared<CMShape>(body, width, height);
}

cm::IShapePtr cm::Factory::CreateShape(cm::IBodyPtr body, const math::Vector2f& first, const math::Vector2f& second, float radius)
{
    return std::make_shared<CMShape>(body, first, second, radius);
}

cm::IShapePtr cm::Factory::CreateShape(cm::IBodyPtr body, const std::vector<math::Vector2f>& vertices, const math::Vector2f& offset)
{
    return std::make_shared<CMShape>(body, vertices, offset);
}
