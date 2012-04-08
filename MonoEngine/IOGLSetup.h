//
//  IOGLSetup.h
//  Monolith1
//
//  Created by Niblit on 2012-04-03.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//


#ifndef MONO_IOGLSETUP_H
#define MONO_IOGLSETUP_H

namespace mono
{
    
    struct IOGLSetup
    {
        virtual ~IOGLSetup()
        { }
        
        virtual void OnCreated() = 0;
        virtual void OnChanged(int width, int height) = 0;
    };
    
}

#endif

