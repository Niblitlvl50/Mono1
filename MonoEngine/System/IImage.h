//
//  IImage.h
//  Monolith1
//
//  Created by Niblit on 2011-12-10.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#pragma once

#include <memory>

typedef unsigned char byte;

namespace mono
{
    
    struct IImage
    {
        virtual ~IImage()
        { }
        
        virtual const byte* Data() const = 0;
        virtual int Width() const = 0;
        virtual int Height() const = 0;
        virtual int ColorComponents() const = 0;
        virtual unsigned int TargetFormat() const = 0;
        virtual bool HasAlpha() const = 0;
    };
    
    typedef std::shared_ptr<IImage> IImagePtr;

}

