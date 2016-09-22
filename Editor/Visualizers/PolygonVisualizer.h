
#pragma once

#include "IDrawable.h"
#include "MonoPtrFwd.h"
#include <vector>

namespace editor
{
    struct Polygon;
    
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
