//
//  IUpdateable.h
//  Monolith1
//
//  Created by Niblit on 2012-03-11.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#pragma once

namespace mono
{
    
    struct IUpdatable
    {
        virtual ~IUpdatable()
        { }
        
        virtual void doUpdate(unsigned int delta) = 0;
    };
    
}

