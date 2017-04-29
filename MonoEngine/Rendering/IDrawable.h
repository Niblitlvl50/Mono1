
#pragma once

#include "MonoFwd.h"
#include "Math/MathFwd.h"

namespace mono
{
    class IDrawable
    {
    public:
    
        virtual ~IDrawable()
        { }

        virtual void doDraw(mono::IRenderer& renderer) const = 0;

        // Bounding box in global coordiantes
        virtual math::Quad BoundingBox() const = 0;
    };
}
