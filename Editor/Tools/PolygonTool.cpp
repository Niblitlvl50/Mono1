//
//  PolygonTool.cpp
//  MonoiOS
//
//  Created by Niklas Damberg on 02/07/16.
//
//

#include "PolygonTool.h"
#include "Editor.h"
#include "Polygon.h"
#include "IRenderer.h"
#include "Color.h"
#include "Math/Quad.h"

using namespace editor;

class PolygonTool::Visualizer : public mono::IDrawable
{
public:

    Visualizer(const math::Vector2f& last_point, const math::Vector2f& mouse_position)
        : m_lastAddedPoint(last_point),
          m_mousePosition(mouse_position)
    { }

    virtual void doDraw(mono::IRenderer& renderer) const
    {
        constexpr mono::Color::RGBA color(1.0f, 0.0f, 0.0f, 0.2f);
        const std::vector<math::Vector2f>& line = { m_lastAddedPoint, m_mousePosition };
        renderer.DrawLines(line, color, 2.0f);
    }

    virtual math::Quad BoundingBox() const
    {
        return math::Quad(-math::INF, -math::INF, math::INF, math::INF);
    }

    const math::Vector2f& m_lastAddedPoint;
    const math::Vector2f& m_mousePosition;
};

PolygonTool::PolygonTool(Editor* editor)
    : m_editor(editor)
{
    m_visualizer = std::make_shared<Visualizer>(m_lastAddedPoint, m_mousePosition);
}

void PolygonTool::Begin()
{
    m_polygon = std::make_shared<editor::PolygonEntity>();
    m_editor->AddPolygon(m_polygon);
    m_firstPoint = true;

    m_editor->AddDrawable(m_visualizer, 1);
}

void PolygonTool::End()
{
    m_polygon = nullptr;
    m_editor->RemoveDrawable(m_visualizer);
}

bool PolygonTool::IsActive() const
{
    return m_polygon != nullptr;
}

void PolygonTool::HandleMouseDown(const math::Vector2f& world_pos, mono::IEntityPtr entity)
{ }

void PolygonTool::HandleMouseUp(const math::Vector2f& world_pos)
{
    if(!m_polygon)
        return;

    if(m_firstPoint)
    {
        m_polygon->SetPosition(world_pos);
        m_polygon->AddVertex(math::zeroVec);

        m_firstPoint = false;
    }
    else
    {
        const math::Vector2f& position = m_polygon->Position();
        m_polygon->AddVertex(world_pos - position);
    }

    m_lastAddedPoint = world_pos;
}

void PolygonTool::HandleMousePosition(const math::Vector2f& world_pos)
{
    m_mousePosition = world_pos;

    if(m_firstPoint)
        m_lastAddedPoint = world_pos;
}
