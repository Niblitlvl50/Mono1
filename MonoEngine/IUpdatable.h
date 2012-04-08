//
//  IUpdateable.h
//  Monolith1
//
//  Created by Niblit on 2012-03-11.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//


#ifndef MONO_IUPDATEABLE_H
#define MONO_IUPDATEABLE_H

namespace mono
{
    
    struct IUpdatable
    {
        virtual ~IUpdatable()
        { }
        
        virtual void Update(unsigned int delta) = 0;
    };
    
}

#endif

