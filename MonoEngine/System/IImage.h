//
//  IImage.h
//  Monolith1
//
//  Created by Niblit on 2011-12-10.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#pragma once

#include <tr1/memory>

typedef unsigned char byte;

namespace mono
{
    
    struct IImage
    {
        virtual ~IImage()
        { }
        
        virtual const byte* Data() = 0;
        virtual int Width() const = 0;
        virtual int Height() const = 0;
        virtual bool HasAlpha() const = 0;
    };
    
    typedef std::tr1::shared_ptr<IImage> IImagePtr;

}

