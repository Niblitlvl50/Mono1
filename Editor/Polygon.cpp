
#include "Polygon.h"
#include "IRenderer.h"
#include "Color.h"
#include "Math/Quad.h"
#include "Math/Matrix.h"
#include "Math/MathFwd.h"
#include "Math/MathFunctions.h"

#include "Texture/TextureFactory.h"

#include <cassert>

namespace
{
    constexpr mono::Color::RGBA line_color(0.0f, 0.0f, 0.0f);
    constexpr mono::Color::RGBA point_color(0.0f, 1.0f, 0.7f);
}

using namespace editor;

PolygonEntity::PolygonEntity()
    : m_selected(false),
      m_texture_name("textures/placeholder.png")
{ }

PolygonEntity::PolygonEntity(const std::vector<math::Vector>& points)
    : PolygonEntity()
{
    mPosition = points.front();

    m_points = points;
    
    for(math::Vector& vertex : m_points)
        vertex -= mPosition;

    RecalculateTextureCoordinates();
    mBasePoint = math::CentroidOfPolygon(m_points);
}

void PolygonEntity::Draw(mono::IRenderer& renderer) const
{
    if(!m_texture)
        return;
    
    constexpr mono::Color::RGBA selected_color(0.0f, 1.0f, 0.0f);
    if(m_selected)
        renderer.DrawClosedPolyline(m_points, selected_color, 6.0f);

    DrawPolygon(renderer, m_texture, m_points, m_textureCoordinates);
    renderer.DrawPoints({ mBasePoint }, point_color, 4.0f);
}

void PolygonEntity::Update(unsigned int delta)
{
    if(!m_texture)
        m_texture = mono::CreateTexture(m_texture_name);
}

math::Quad PolygonEntity::BoundingBox() const
{
    const math::Matrix& transform = Transformation();

    math::Quad bb(math::INF, math::INF, -math::INF, -math::INF);

    for(auto& point : m_points)
        bb |= math::Transform(transform, point);

    return bb;
}

void PolygonEntity::AddVertex(const math::Vector& vertex)
{
    m_points.push_back(vertex);
    RecalculateTextureCoordinates();
    mBasePoint = math::CentroidOfPolygon(m_points);
}

void PolygonEntity::SetVertex(const math::Vector& vertex, size_t index)
{
    math::Matrix transform = Transformation();
    math::Inverse(transform);

    m_points[index] = math::Transform(transform, vertex);
    RecalculateTextureCoordinates();
    //mBasePoint = math::CentroidOfPolygon(m_points);
}

const std::vector<math::Vector>& PolygonEntity::GetVertices() const
{
    return m_points;
}

void PolygonEntity::SetSelected(bool selected)
{
    m_selected = selected;
}

bool PolygonEntity::IsSelected() const
{
    return m_selected;
}

void PolygonEntity::SetTexture(const char* texture)
{
    const std::size_t length = std::strlen(texture);
    assert(length <= 32);

    std::memset(m_texture_name, 0, 32);
    std::memcpy(m_texture_name, texture, length);

    m_texture = nullptr;
}

const char* PolygonEntity::GetTexture() const
{
    return m_texture_name;
}

math::Quad PolygonEntity::LocalBoundingBox() const
{
    math::Quad bb(math::INF, math::INF, -math::INF, -math::INF);

    for(auto& point : m_points)
        bb |= point;

    return bb;
}

void PolygonEntity::RecalculateTextureCoordinates()
{
    m_textureCoordinates.clear();

    const math::Quad& bb = LocalBoundingBox();
    const math::Vector& repeate = (bb.mB - bb.mA) / 2.0f;

    for(const math::Vector& point : m_points)
        m_textureCoordinates.push_back(math::MapVectorInQuad(point, bb) * repeate);
}

void editor::DrawPolygon(mono::IRenderer& renderer,
                         const mono::ITexturePtr& texture,
                         const std::vector<math::Vector>& points,
                         const std::vector<math::Vector>& texture_coords)
{
    std::vector<unsigned short> indices;
    for(size_t index = 0; index < points.size(); ++index)
        indices.push_back(index);

    renderer.DrawGeometry(points, texture_coords, indices, texture);
    renderer.DrawClosedPolyline(points, line_color, 2.0f);
    renderer.DrawPoints(points, point_color, 4.0f);
}

