//
//  IZone.h
//  Monolith1
//
//  Created by Niblit on 2012-03-11.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#ifndef MONO_IZONE_H
#define MONO_IZONE_H

#include "MonoFwd.h"

namespace mono
{
    
    struct IZone
    {
        virtual ~IZone()
        { }
        
        virtual void Accept(IRenderer& renderer) = 0;
        virtual bool IsDone() const = 0;
        //virtual void Update() = 0;
    };

}

#endif

