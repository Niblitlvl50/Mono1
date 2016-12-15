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
#include "Math/Quad.h"
#include "Math/Matrix.h"
#include "Math/MathFwd.h"
#include "Math/MathFunctions.h"

#include "Texture/TextureFactory.h"

#include <cassert>

using namespace editor;

PolygonEntity::PolygonEntity()
    : m_selected(false),
      m_texture_repeate(1.0f),
      m_texture_name("textures/placeholder.png")
{ }

void PolygonEntity::Draw(mono::IRenderer& renderer) const
{
    if(!m_texture)
        return;
    
    constexpr mono::Color::RGBA line_color(0.0f, 0.0f, 0.0f);
    constexpr mono::Color::RGBA point_color(0.0f, 1.0f, 0.7f);
    constexpr mono::Color::RGBA selected_color(0.4f, 0.4f, 0.4f);

    std::vector<unsigned short> indices;

    for(size_t index = 0; index < m_points.size(); ++index)
        indices.push_back(index);

    renderer.DrawGeometry(m_points, m_textureCoordinates, indices, m_texture);

    renderer.DrawClosedPolyline(m_points, line_color, 2.0f);
    renderer.DrawPoints(m_points, point_color, 4.0f);

    renderer.DrawPoints({ mBasePoint }, point_color, 4.0f);

    if(m_selected)
    {
        const math::Quad& bb = LocalBoundingBox();
        renderer.DrawQuad(bb, selected_color, 1.0f);
    }
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

    if(m_points.size() > 2)
        mBasePoint = math::CentroidOfPolygon(m_points);
}

void PolygonEntity::SetVertex(const math::Vector& vertex, size_t index)
{
    math::Matrix transform = Transformation();
    math::Inverse(transform);

    m_points[index] = math::Transform(transform, vertex);
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

void PolygonEntity::SetTextureRepeate(float repeate)
{
    m_texture_repeate = repeate;
    RecalculateTextureCoordinates();
}

float PolygonEntity::GetTextureRepate() const
{
    return m_texture_repeate;
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
    for(const math::Vector& point : m_points)
        m_textureCoordinates.push_back(math::MapVectorInQuad(point, bb) * m_texture_repeate);
}


