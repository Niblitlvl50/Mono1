
#pragma once

#include "Math/MathFwd.h"

namespace mono
{
    class IRenderer;

    class IDrawable
    {
    public:
    
        virtual ~IDrawable() = default;
        virtual void Draw(mono::IRenderer& renderer) const = 0;

        // Bounding box in world coordiantes, axis aligned.
        virtual math::Quad BoundingBox() const = 0;
    };
}
