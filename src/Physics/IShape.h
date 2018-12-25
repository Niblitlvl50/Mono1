
#pragma once

#include "CMFwd.h"

namespace mono
{
    class IShape
    {
    public:

        virtual ~IShape()
        { }

        //! How bouncy this shape is.
        //! 0.0 means no bounce, 1.0 means perfect bounce
        virtual void SetElasticity(float value) = 0;

        //! How much friction this shape has.
        virtual void SetFriction(float value) = 0;

        //! Get the calculated inertia value for this shape
        virtual float GetInertiaValue() const = 0;

        //! Category that the shape belongs to, and a bitmask with which other
        //! categories the shape should collide with
        virtual void SetCollisionFilter(unsigned int category, unsigned int mask) = 0;

        //! Native chipmunk handle
        virtual cpShape* Handle() = 0;
    };
}
