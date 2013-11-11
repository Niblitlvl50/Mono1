//
//  IPhysicsEntity.h
//  King_worksample_OSX
//
//  Created by Niklas Damberg on 11/11/13.
//
//

#pragma once

#include "IEntity.h"
#include "CMFwd.h"

namespace mono
{
    struct IPhysicsEntity : public IEntity
    {
        virtual ~IPhysicsEntity()
        { }
        
        virtual cm::Object& GetPhysics() = 0;
    };
}
