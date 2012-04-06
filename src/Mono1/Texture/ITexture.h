//
//  ITexture.h
//  Monolith1
//
//  Created by Niblit on 2011-12-10.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//


#ifndef MONO_ITEXTURE_H
#define MONO_ITEXTURE_H

namespace mono
{
    
    struct ITexture
    {
        virtual ~ITexture()
        { }
        
        virtual void Use() = 0;
    };

}

#endif

