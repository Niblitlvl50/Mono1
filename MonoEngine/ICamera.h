//
//  ICamera.h
//  Monolith1
//
//  Created by Niblit on 2012-03-11.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//


#pragma once

#include "MonoPtrFwd.h"
#include "MathFwd.h"

namespace mono
{
    
    struct ICamera
    {        
        virtual ~ICamera()
        { }
        
        virtual void Update(unsigned int delta) = 0;
                
        virtual void Follow(const mono::IEntityPtr entity) = 0;
        virtual void Unfollow() = 0;
        
        virtual const math::Quad& GetViewport() const = 0;
        virtual /* */ math::Quad& GetViewport() /* */ = 0;
        
        virtual void SetTargetViewport(const math::Quad& target) = 0;
        virtual void SetPosition(const math::Vector2f& position) = 0;
    };
    
}


