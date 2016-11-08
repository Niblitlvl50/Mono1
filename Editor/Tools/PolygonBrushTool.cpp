//
//  PolygonBrushTool.cpp
//  MonoiOS
//
//  Created by Niklas Damberg on 09/08/16.
//
//

#include "PolygonBrushTool.h"
#include "Editor.h"
#include "Polygon.h"

#include "Math/MathFunctions.h"
#include "Math/Quad.h"

#include "IDrawable.h"
#include "IRenderer.h"
#include "Color.h"

#include <cmath>

using namespace editor;

class PolygonBrushTool::Visualizer : public mono::IDrawable
{
public:

    Visualizer(const bool& painting, const std::vector<math::Vector2f>& drawn_points)
        : m_painting(painting),
          m_points(drawn_points)
    { }

    virtual void doDraw(mono::IRenderer& renderer) const
    {
        if(!m_painting)
            return;

        constexpr mono::Color::RGBA color(0.0f, 1.0f, 0.5f, 1.0f);
        renderer.DrawPoints(m_points, color, 2.0f);
    }

    virtual math::Quad BoundingBox() const
    {
        return math::Quad(-math::INF, -math::INF, math::INF, math::INF);
    }

    const bool& m_painting;
    const std::vector<math::Vector2f>& m_points;
};

PolygonBrushTool::PolygonBrushTool(Editor* editor)
    : m_editor(editor),
      m_painting(false)
{
    m_visualizer = std::make_shared<PolygonBrushTool::Visualizer>(m_painting, m_drawnPoints);
}

void PolygonBrushTool::Begin()
{
    m_painting = false;
    m_direction = math::INF;

    m_polygon = std::make_shared<editor::PolygonEntity>();
    m_editor->AddPolygon(m_polygon);

    m_editor->AddDrawable(m_visualizer, 1);
}

void PolygonBrushTool::End()
{
    m_editor->RemoveDrawable(m_visualizer);

    m_painting = false;
    m_polygon = nullptr;
    m_drawnPoints.clear();
}

bool PolygonBrushTool::IsActive() const
{
    return m_painting;
}

void PolygonBrushTool::HandleMouseDown(const math::Vector2f& world_pos, mono::IEntityPtr entity)
{
    if(!m_polygon)
        return;

    m_painting = true;
    m_previousPoint = world_pos;
    m_previouslyAddedPoint = world_pos;

    m_polygon->SetPosition(world_pos);
    m_polygon->AddVertex(math::zeroVec);
}

void PolygonBrushTool::HandleMouseUp(const math::Vector2f& world_pos)
{
    if(IsActive())
        End();
}

void PolygonBrushTool::HandleMousePosition(const math::Vector2f& world_pos)
{
    if(!m_painting)
        return;

    if(m_direction == math::INF)
    {
        m_direction = math::AngleBetweenPoints(m_previouslyAddedPoint, world_pos);
        return;
    }

    constexpr float threashold = math::ToRadians(5.0f);
    const float angle = std::fabs(m_direction - math::AngleBetweenPoints(m_previouslyAddedPoint, world_pos));

    const math::Vector2f& position_diff = world_pos - m_previouslyAddedPoint;
    const float distance = math::Length(position_diff);

    if(angle > threashold && distance > 5.0f)
    {
        const math::Vector2f& position = m_polygon->Position();
        m_polygon->AddVertex(m_previousPoint - position);

        m_previouslyAddedPoint = m_previousPoint;
        m_direction = math::AngleBetweenPoints(m_previouslyAddedPoint, world_pos);
    }

    m_previousPoint = world_pos;

    m_drawnPoints.push_back(m_previousPoint);
}
