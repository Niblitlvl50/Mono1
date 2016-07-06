//
//  Polygon.cpp
//  MonoiOS
//
//  Created by Niklas Damberg on 25/06/16.
//
//


#include "Polygon.h"
#include "IRenderer.h"
#include "Color.h"
#include "Quad.h"

#include "MathFwd.h"

using namespace editor;

PolygonEntity::PolygonEntity()
    : m_selected(false)
{ }

void PolygonEntity::Draw(mono::IRenderer& renderer) const
{
    constexpr mono::Color::RGBA line_color(1.0f, 0.0f, 0.7f);
    constexpr mono::Color::RGBA point_color(0.0f, 1.0f, 0.7f);
    constexpr mono::Color::RGBA selected_color(0.8f, 0.8f, 0.8f);

    renderer.DrawClosedPolyline(m_points, line_color, 2.0f);
    renderer.DrawPoints(m_points, point_color, 3.0f);

    if(m_selected)
    {
        math::Quad bb = BoundingBox();
        bb.mA -= Position();
        bb.mB -= Position();
        renderer.DrawQuad(bb, selected_color, 1.0f);
    }
}

void PolygonEntity::Update(unsigned int delta)
{ }

math::Quad PolygonEntity::BoundingBox() const
{
    math::Quad bb(math::INF, math::INF, -math::INF, -math::INF);

    for(auto& point : m_points)
        bb |= (point + Position());

    return bb;
}

