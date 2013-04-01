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
        
        //! Update function
        //! @param[in] delta Delta value of how many milliseconds since last update.
        virtual void doUpdate(unsigned int delta) = 0;
    };
    
}

