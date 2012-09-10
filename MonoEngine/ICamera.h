//
//  ICamera.h
//  Monolith1
//
//  Created by Niblit on 2012-03-11.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//


#pragma once

#include "MonoFwd.h"

namespace mono
{
    
    struct ICamera
    {        
        virtual ~ICamera()
        { }
        
        virtual void Update(unsigned int delta) = 0;
                
        virtual void Follow(const mono::IEntityPtr entity) = 0;
        virtual void Unfollow() = 0;
        
        virtual const Math::Quad& GetViewport() const = 0;
        virtual /* */ Math::Quad& GetViewport() /* */ = 0;
        
        virtual void SetTargetViewport(const Math::Quad& target) = 0;
        virtual void SetPosition(const Math::Vector2f& position) = 0;
    };
    
}


