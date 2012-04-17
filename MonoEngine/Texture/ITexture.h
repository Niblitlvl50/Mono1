//
//  ITexture.h
//  Monolith1
//
//  Created by Niblit on 2011-12-10.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//


#pragma once

namespace mono
{
    
    struct ITexture
    {
        virtual ~ITexture()
        { }
        
        virtual void Use() const = 0;
        virtual int Width() const = 0;
        virtual int Height() const = 0;
    };

}

