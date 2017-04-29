
#pragma once

#include "Rendering/IDrawable.h"
#include "MonoPtrFwd.h"
#include "Math/Vector.h"
#include <vector>

namespace editor
{
    struct Polygon
    {
        std::vector<math::Vector> vertices;
        std::vector<math::Vector> texture_coordinates;
        math::Vector centroid;
        float texture_repeate;
    };
    
    class PolygonVisualizer : public mono::IDrawable
    {
    public:

        PolygonVisualizer(const std::vector<Polygon>& polygons);

        virtual void doDraw(mono::IRenderer& renderer) const;
        virtual math::Quad BoundingBox() const;

        const std::vector<Polygon>& m_polygons;
        mono::ITexturePtr m_texture;
    };
}
