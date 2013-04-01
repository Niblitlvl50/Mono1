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
            cpBodyDestroy(mBody);
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
        }
        CMShape(cm::IBodyPtr body, float width, float height)
        {
            mShape = cpBoxShapeNew(body->Body(), width, height);
        }
        CMShape(cm::IBodyPtr body, const math::Vector2f& first, const math::Vector2f& second, float radius)
        {
            mShape = cpSegmentShapeNew(body->Body(), cpv(first.mX, first.mY), cpv(second.mX, second.mY), radius);
        }
        CMShape(cm::IBodyPtr body, const Vector2fCollection& vertices, const math::Vector2f& offset)
        {
            std::vector<cpVect> vects;
            for(Vector2fCollection::const_iterator it = vertices.begin(), end = vertices.end(); it != end; ++it)
            {
                const math::Vector2f& vect2f = *it;
                vects.push_back(cpv(vect2f.mX, vect2f.mY));
            }
            
            mShape = cpPolyShapeNew(body->Body(), vertices.size(), &vects.front(), cpv(offset.mX, offset.mY));
        }
        ~CMShape()
        {
            cpShapeDestroy(mShape);
        }
        void SetElasticity(float value)
        {
            cpShapeSetElasticity(mShape, value);
        }
        void SetFriction(float value)
        {
            cpShapeSetFriction(mShape, value);
        }
        cpShape* Shape()
        {
            return mShape;
        }
        
        cpShape* mShape;    
    };
}

cm::IBodyPtr cm::Factory::CreateStaticBody()
{
    return IBodyPtr(new CMBody);
}

cm::IBodyPtr cm::Factory::CreateBody(float mass, float inertia)
{
    return IBodyPtr(new CMBody(mass, inertia));
}

cm::IShapePtr cm::Factory::CreateShape(cm::IBodyPtr body, float radius, const math::Vector2f& offset)
{
    return IShapePtr(new CMShape(body, radius, offset));
}

cm::IShapePtr cm::Factory::CreateShape(cm::IBodyPtr body, float width, float height)
{
    return IShapePtr(new CMShape(body, width, height));
}

cm::IShapePtr cm::Factory::CreateShape(cm::IBodyPtr body, const math::Vector2f& first, const math::Vector2f& second, float radius)
{
    return IShapePtr(new CMShape(body, first, second, radius));
}

cm::IShapePtr cm::Factory::CreateShape(cm::IBodyPtr body, const Vector2fCollection& vertices, const math::Vector2f& offset)
{
    return IShapePtr(new CMShape(body, vertices, offset));
}
