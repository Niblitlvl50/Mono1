//
//  CMIBody.h
//  Mono1
//
//  Created by Niblit on 2012-08-24.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#pragma once

#include "CMFwd.h"

namespace cm
{
    struct IBody
    {
        virtual ~IBody()
        { }
        
        virtual void SetMass(float mass) = 0;
        virtual void SetAngle(float angle) = 0;
        virtual float GetAngle() const = 0;
        virtual void SetMoment(float moment) = 0;
        virtual void SetPosition(const Math::Vector2f& position) = 0;
        virtual Math::Vector2f GetPosition() const = 0;
        
        virtual void ApplyForce(const Math::Vector2f& force, const Math::Vector2f& offset) = 0;
        virtual void ApplyImpulse(const Math::Vector2f& impulse, const Math::Vector2f& offset) = 0;
        
        virtual void ResetForces() = 0;
        
        virtual void OnCollideWith(IBodyPtr body) = 0;

        virtual cpBody* Body() = 0;
        
    private:
        
        friend class Factory;
        friend class Space;
    };
}

