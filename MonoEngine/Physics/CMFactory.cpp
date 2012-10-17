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
        CMBody(float mass, float inertia)
        {
            mBody = cpBodyNew(mass, inertia);
        }
        ~CMBody()
        {
            cpBodyDestroy(mBody);
        }
        void SetMass(float mass)
        {
            cpBodySetMass(mBody, mass);
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
        void SetPosition(const Math::Vector2f& position)
        {
            cpBodySetPos(mBody, cpv(position.mX, position.mY));
        }
        Math::Vector2f GetPosition() const
        {
            return Math::Vector2f(mBody->p.x, mBody->p.y);
        }
        void ApplyForce(const Math::Vector2f& force, const Math::Vector2f& offset)
        {
            cpBodyApplyForce(mBody, cpv(force.mX, force.mY), cpv(offset.mX, offset.mY));
        }
        void ApplyImpulse(const Math::Vector2f& impulse, const Math::Vector2f& offset)
        {
            cpBodyApplyImpulse(mBody, cpv(impulse.mX, impulse.mY), cpv(offset.mX, offset.mY));
        }
        void ResetForces()
        {
            cpBodyResetForces(mBody);
        }
        cpBody* Body()
        {
            return mBody;
        }
        
        cpBody* mBody;
    };
    
    struct CMShape : cm::IShape
    {
        CMShape(cpBody* body, float radius, const Math::Vector2f& offset, bool staticShape)
            : mStaticShape(staticShape)
        {
            mShape = cpCircleShapeNew(body, radius, cpv(offset.mX, offset.mY));
        }
        CMShape(cpBody* body, float width, float height, bool staticShape)
            : mStaticShape(staticShape)
        {
            mShape = cpBoxShapeNew(body, width, height);
        }
        CMShape(cpBody* body, const Math::Vector2f& first, const Math::Vector2f& second, float radius, bool staticShape)
            : mStaticShape(staticShape)
        {
            mShape = cpSegmentShapeNew(body, cpv(first.mX, first.mY), cpv(second.mX, second.mY), radius);
        }
        CMShape(cpBody* body, const Vector2fCollection& vertices, const Math::Vector2f& offset, bool staticShape)
            : mStaticShape(staticShape)
        {
            std::vector<cpVect> vects;
            for(Vector2fCollection::const_iterator it = vertices.begin(), end = vertices.end(); it != end; ++it)
            {
                const Math::Vector2f& vect2f = *it;
                vects.push_back(cpv(vect2f.mX, vect2f.mY));
            }
            
            mShape = cpPolyShapeNew(body, vertices.size(), &vects.front(), cpv(offset.mX, offset.mY));
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
        bool IsStatic() const
        {
            return mStaticShape;
        }
        cpShape* Shape()
        {
            return mShape;
        }
        
        const bool mStaticShape;
        cpShape* mShape;    
    };
}

cm::IBodyPtr cm::Factory::CreateBody(float mass, float inertia)
{
    return IBodyPtr(new CMBody(mass, inertia));
}

cm::IShapePtr cm::Factory::CreateShape(cm::IBodyPtr body, float radius, const Math::Vector2f& offset, bool staticShape)
{
    return IShapePtr(new CMShape(body->Body(), radius, offset, staticShape));
}

cm::IShapePtr cm::Factory::CreateShape(cm::IBodyPtr body, float width, float height, bool staticShape)
{
    return IShapePtr(new CMShape(body->Body(), width, height, staticShape));
}

cm::IShapePtr cm::Factory::CreateShape(cm::IBodyPtr body, const Math::Vector2f& first, const Math::Vector2f& second, float radius, bool staticShape)
{
    return IShapePtr(new CMShape(body->Body(), first, second, radius, staticShape));
}

cm::IShapePtr cm::Factory::CreateShape(cm::IBodyPtr body, const Vector2fCollection& vertices, const Math::Vector2f& offset, bool staticShape)
{
    return IShapePtr(new CMShape(body->Body(), vertices, offset, staticShape));
}
