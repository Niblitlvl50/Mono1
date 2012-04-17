//
//  IEntity.h
//  Mono1
//
//  Created by Niblit on 2012-04-06.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//


#pragma once

#include "IDrawable.h"
#include "IUpdatable.h"

namespace mono
{
    struct IEntity : public IDrawable, public IUpdatable
    {
        virtual ~IEntity()
        { }
        
        //virtual unsigned int GetId() const = 0;
    };
}

