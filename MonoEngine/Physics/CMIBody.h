//
//  CMIBody.h
//  Mono1
//
//  Created by Niblit on 2012-08-24.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#pragma once

#include "CMFwd.h"

#include <tr1/memory>

namespace cm
{
    struct ICollisionHandler
    {
        virtual ~ICollisionHandler()
        { }
        
        virtual void OnCollideWith(cm::IBodyPtr body) = 0;
    };
    
    //std::tr1::shared_ptr<ICollisionHandler> ICollisionHandlerPtr;
    
    struct IBody
    {
        virtual ~IBody()
        { }
        
        //! Checks if this is a static body or not
        //! @return bool
        virtual bool IsStatic() const = 0;
        
        //! Sets the bodys mass
        //! @param mass The new mass
        virtual void SetMass(float mass) = 0;
        
        //! Get this bodys mass
        //! @return float The mass
        virtual float GetMass() const = 0;
        
        //! Sets the angle
        //! @param angle New angle
        virtual void SetAngle(float angle) = 0;
        
        //! Gets the angle
        //! @return float Angle
        virtual float GetAngle() const = 0;
        
        //! Sets the moment(inertia)
        //! @param moment 
        virtual void SetMoment(float moment) = 0;
        
        //! Gets the moment(inertia) of the body
        //! @return float moment
        virtual float GetMoment() const = 0;
        
        //! Set the position of the body
        //! @param position A new position
        virtual void SetPosition(const math::Vector2f& position) = 0;
        
        //! Gets the bodys position
        //! @return math::Vector2f The position
        virtual math::Vector2f GetPosition() const = 0;
        
        //! Apply a force to the body
        //! @param force The force
        //! @param offset From where to apply it
        virtual void ApplyForce(const math::Vector2f& force, const math::Vector2f& offset) = 0;
        
        //! Apply an impulse to the body
        //! @param impulse The impulse
        //! @param offset From where to apply it
        virtual void ApplyImpulse(const math::Vector2f& impulse, const math::Vector2f& offset) = 0;
        
        //! Resets all the forces that are working on the body
        virtual void ResetForces() = 0;
        
        virtual void SetCollisionHandler(cm::ICollisionHandler* handler) = 0;
        
        virtual void OnCollideWith(IBodyPtr body) = 0;
        
        //! Gets the underlying cpBody object, dont use this one
        //! @return cpBody* A pointer to a cpBody struct
        virtual cpBody* Body() = 0;
    };
}

