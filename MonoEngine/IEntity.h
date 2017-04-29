
#pragma once

#include "Rendering/IDrawable.h"
#include "IUpdatable.h"
#include "System/SysTypes.h"
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
        virtual void SetScale(const math::Vector& scale) = 0;

        // Rotation in radians
        virtual float Rotation() const = 0;
        virtual void SetRotation(float rotation) = 0;

        virtual math::Matrix Transformation() const = 0;

        // Unique id
        virtual uint Id() const = 0;

        virtual void SetProperty(uint property) = 0;
        virtual bool HasProperty(uint property) const = 0;
    };
}
