//
//  CMIShape.h
//  Mono1
//
//  Created by Niblit on 2012-08-24.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#pragma once

#include "CMFwd.h"

namespace mono
{
    struct IShape
    {
        virtual ~IShape()
        { }
        
        //! How bouncy this shape is.
        //! 0.0 means no bounce, 1.0 means perfect bounce
        virtual void SetElasticity(float value) = 0;

        //! How much friction this shape has.
        virtual void SetFriction(float value) = 0;

        //! Get the calculated inertia value for this shape
        virtual float GetInertiaValue() const = 0;

    private:
        
        friend class Space;
        virtual cpShape* Shape() = 0;
    };
}

