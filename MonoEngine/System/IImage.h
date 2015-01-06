//
//  IImage.h
//  Monolith1
//
//  Created by Niblit on 2011-12-10.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#pragma once

#include <memory>
#include "SysTypes.h"

namespace mono
{
    struct IImage
    {
        virtual ~IImage()
        { }

        //! Get the raw image data
        //! \return byte* Pointer to the data
        virtual const byte* Data() const = 0;

        //! Return the width
        virtual unsigned int Width() const = 0;

        //! Return the height
        virtual unsigned int Height() const = 0;

        //! I dont know!
        virtual int ColorComponents() const = 0;

        //! I dont know!
        virtual unsigned int TargetFormat() const = 0;

        //! Check if the image has alpha channel
        virtual bool HasAlpha() const = 0;
    };
    
    typedef std::shared_ptr<IImage> IImagePtr;
}

