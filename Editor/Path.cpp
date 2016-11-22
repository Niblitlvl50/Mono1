
#include "Path.h"
#include "IRenderer.h"
#include "Color.h"
#include "Math/Quad.h"
#include "Math/Matrix.h"
#include "Math/MathFunctions.h"

using namespace editor;

PathEntity::PathEntity(const std::string& name, const std::vector<math::Vector2f>& points)
    : m_name(name),
      m_points(points),
      m_selected(false)
{
    mBasePoint = math::CentroidOfPolygon(m_points);
}

void PathEntity::Draw(mono::IRenderer& renderer) const
{
    constexpr mono::Color::RGBA line_color(1.0f, 0.0f, 0.0f, 0.2f);
    constexpr mono::Color::RGBA point_color(1.0f, 0.0f, 1.0f, 0.8f);
    constexpr mono::Color::RGBA selected_color(0.4f, 0.4f, 0.4f);

    renderer.DrawPolyline(m_points, line_color, 2.0f);
    renderer.DrawPoints(m_points, point_color, 4.0f);
    renderer.DrawPoints( { mBasePoint }, point_color, 4.0f);

    if(m_selected)
        renderer.DrawQuad(LocalBoundingBox(), selected_color, 1.0f);
}

void PathEntity::Update(unsigned int delta)
{ }

math::Quad PathEntity::BoundingBox() const
{
    const math::Matrix& transform = Transformation();

    math::Quad bb(math::INF, math::INF, -math::INF, -math::INF);

    for(auto& point : m_points)
        bb |= math::Transform(transform, point);

    return bb;
}

math::Quad PathEntity::LocalBoundingBox() const
{
    math::Quad bb(math::INF, math::INF, -math::INF, -math::INF);

    for(auto& point : m_points)
        bb |= point;

    return bb;
}

void PathEntity::SetSelected(bool selected)
{
    m_selected = selected;
}

void PathEntity::SetVertex(const math::Vector2f& vertex, size_t index)
{
    m_points[index] = vertex;
}
