
#pragma once

#include "Math/MathFwd.h"

#define ENUM_BIT(n) (1 << (n))

namespace mono
{
    class IRenderer;

    enum DrawableProperty
    {
        DP_DEFAULT = 0,
        DP_POST_LIGHTING = ENUM_BIT(0),
    };

    class IDrawable
    {
    public:
    
        virtual ~IDrawable() = default;
        virtual void Draw(mono::IRenderer& renderer) const = 0;

        // Bounding box in world coordiantes, axis aligned.
        virtual math::Quad BoundingBox() const = 0;
        virtual int DrawProperties() const { return DP_DEFAULT; }
    };
}
