
#pragma once

#include "MonoFwd.h"
#include "Math/MathFwd.h"

namespace mono
{
    struct IDrawable
    {
        virtual ~IDrawable()
        { }

        virtual void doDraw(mono::IRenderer& renderer) const = 0;

        // Bounding box in global coordiantes
        virtual math::Quad BoundingBox() const = 0;
    };
}