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
#include "Matrix.h"

#include "MathFwd.h"
#include "MathFunctions.h"

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

    renderer.DrawPoints({ m_centroid }, point_color, 4.0f);

    if(m_selected)
    {
        const math::Quad& bb = LocalBoundingBox();
        renderer.DrawQuad(bb, selected_color, 1.0f);
    }
}

void PolygonEntity::Update(unsigned int delta)
{ }

math::Quad PolygonEntity::BoundingBox() const
{
    const math::Matrix& transform = Transformation();

    math::Quad bb(math::INF, math::INF, -math::INF, -math::INF);

    for(auto& point : m_points)
        bb |= math::Transform(transform, point);

    return bb;
}

void PolygonEntity::AddVertex(const math::Vector2f& vertex)
{
    m_points.push_back(vertex);

    if(m_points.size() > 2)
    {
        m_centroid = math::CentroidOfPolygon(m_points);
        mBasePoint = m_centroid;
    }
}

const std::vector<math::Vector2f>& PolygonEntity::GetVertices() const
{
    return m_points;
}

void PolygonEntity::SetSelected(bool selected)
{
    m_selected = selected;
}

math::Quad PolygonEntity::LocalBoundingBox() const
{
    math::Quad bb(math::INF, math::INF, -math::INF, -math::INF);

    for(auto& point : m_points)
        bb |= point;

    return bb;
}

