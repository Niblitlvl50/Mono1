
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

        virtual void SetCollisionType(unsigned int type) = 0;

        virtual cpShape* Handle() = 0;
    };
}
