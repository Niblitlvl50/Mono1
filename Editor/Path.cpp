
#include "Path.h"
#include "IRenderer.h"
#include "Color.h"
#include "Math/Quad.h"
#include "Math/Matrix.h"
#include "Math/MathFunctions.h"

using namespace editor;

PathEntity::PathEntity(const std::string& name)
    : m_name(name),
      m_selected(false)
{ }

PathEntity::PathEntity(const std::string& name, const std::vector<math::Vector>& points)
    : PathEntity(name)
{
      m_points = points;
}

void PathEntity::Draw(mono::IRenderer& renderer) const
{
    constexpr mono::Color::RGBA line_color(0.0f, 0.0f, 0.0f, 1.0f);
    constexpr mono::Color::RGBA point_color(0.0f, 1.0f, 0.7f);
    constexpr mono::Color::RGBA selected_color(0.0f, 1.0f, 0.0f);

    if(m_selected)
        renderer.DrawPolyline(m_points, selected_color, 5.0f);

    renderer.DrawPolyline(m_points, line_color, 2.0f);
    renderer.DrawPoints(m_points, point_color, 4.0f);
    renderer.DrawPoints( { mBasePoint }, point_color, 4.0f);
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

void PathEntity::AddVertex(const math::Vector& vertex)
{
    if(m_points.empty())
    {
        SetPosition(vertex);
        m_points.push_back(math::zeroVec);
        return;
    }

    m_points.push_back(vertex - Position());
    if(m_points.size() > 2)
        mBasePoint = math::CentroidOfPolygon(m_points);
}

void PathEntity::SetVertex(const math::Vector& vertex, size_t index)
{
    math::Matrix transform = Transformation();
    math::Inverse(transform);

    m_points[index] = math::Transform(transform, vertex);
}

void PathEntity::SetName(const char* new_name)
{
    m_name = new_name;
}
