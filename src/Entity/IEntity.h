
#pragma once

#include "Rendering/IDrawable.h"
#include "IUpdatable.h"
#include "Math/MathFwd.h"

namespace mono
{
    class IEntity : public IDrawable, public IUpdatable
    {
    public:

        virtual ~IEntity()
        { }

        virtual const math::Vector& Position() const = 0;
        virtual const math::Vector& BasePoint() const = 0;
        virtual const math::Vector& Scale() const = 0;

        virtual void SetPosition(const math::Vector& position) = 0;
        virtual void SetBasePoint(const math::Vector& base_point) = 0;
        virtual void SetScale(const math::Vector& scale) = 0;

        // Rotation in radians.
        virtual float Rotation() const = 0;
        virtual void SetRotation(float rotation) = 0;

        // Local to world.
        virtual math::Matrix Transformation() const = 0;

        // Unique id.
        virtual unsigned int Id() const = 0;
        virtual void SetId(unsigned int new_id) = 0;

        // 32 bits bitset.
        virtual void SetProperty(unsigned int property) = 0;
        virtual bool HasProperty(unsigned int property) const = 0;
    };
}
