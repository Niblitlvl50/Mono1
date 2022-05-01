
#pragma once

#include <cstdint>

struct cpShape;

namespace mono
{
    class IShape
    {
    public:

        virtual ~IShape() = default;

        //! How bouncy this shape is.
        //! 0.0 means no bounce, 1.0 means perfect bounce
        virtual void SetElasticity(float value) = 0;

        //! How much friction this shape has.
        virtual void SetFriction(float value) = 0;

        //! Get the calculated inertia value for this shape
        virtual float GetInertiaValue() const = 0;

        virtual void SetSensor(bool is_sensor) = 0;
        virtual bool IsSensor() const = 0;

        //! Category that the shape belongs to, and a bitmask with which other
        //! categories the shape should collide with
        virtual void SetCollisionFilter(uint32_t category, uint32_t mask) = 0;
        virtual void SetCollisionMask(uint32_t mask) = 0;

        virtual void SetCollisionBit(uint32_t collision_category) = 0;
        virtual void ClearCollisionBit(uint32_t collision_category) = 0;

        //! Native chipmunk handle
        virtual cpShape* Handle() const = 0;
    };
}
