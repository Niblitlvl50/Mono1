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

        //! Use the texture
        virtual void Use() const = 0;

        //! Get the width of the texture
        //! \return int
        virtual unsigned int Width() const = 0;

        //! Get the height of the texture
        //! \return int
        virtual unsigned int Height() const = 0;
    };
}

