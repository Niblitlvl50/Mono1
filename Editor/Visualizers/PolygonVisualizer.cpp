//
//  PolygonVisualizer.cpp
//  MonoiOS
//
//  Created by Niklas Damberg on 03/09/16.
//
//

#include "PolygonVisualizer.h"
#include "Texture/TextureFactory.h"
#include "Math/Quad.h"
#include "Color.h"
#include "IRenderer.h"
#include "Polygon.h"

using namespace editor;

PolygonVisualizer::PolygonVisualizer(const std::vector<Polygon>& polygons)
    : m_polygons(polygons)
{
    m_texture = mono::CreateTexture("placeholder.png");
}

void PolygonVisualizer::doDraw(mono::IRenderer& renderer) const
{
    for(const Polygon& polygon : m_polygons)
    {
        constexpr mono::Color::RGBA line_color(1.0f, 0.0f, 0.7f);
        constexpr mono::Color::RGBA point_color(0.0f, 1.0f, 0.7f);
        constexpr mono::Color::RGBA selected_color(0.8f, 0.8f, 0.8f);

        std::vector<unsigned short> indices;
        for(size_t index = 0; index < polygon.vertices.size(); ++index)
            indices.push_back(index);

        renderer.DrawGeometry(polygon.vertices, polygon.texture_coordinates, indices, m_texture);

        renderer.DrawClosedPolyline(polygon.vertices, line_color, 2.0f);
        renderer.DrawPoints(polygon.vertices, point_color, 3.0f);

        renderer.DrawPoints({ polygon.centroid }, point_color, 4.0f);

        /*
        if(m_selected)
        {
            const math::Quad& bb = LocalBoundingBox();
            renderer.DrawQuad(bb, selected_color, 1.0f);
        }
         */
    }
}

math::Quad PolygonVisualizer::BoundingBox() const
{
    return math::Quad(-math::INF, -math::INF, math::INF, math::INF);
}
