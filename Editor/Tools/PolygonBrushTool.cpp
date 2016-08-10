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

#include "MathFunctions.h"

#include <cmath>

using namespace editor;

PolygonBrushTool::PolygonBrushTool(EditorZone* editor)
    : m_editor(editor)
{ }

void PolygonBrushTool::Begin()
{
    m_painting = false;
    m_direction = math::INF;

    m_polygon = std::make_shared<editor::PolygonEntity>();
    m_editor->AddPolygon(m_polygon);
}

void PolygonBrushTool::End()
{
    m_painting = false;
    m_polygon = nullptr;
}

bool PolygonBrushTool::IsActive() const
{
    return m_painting;
}

void PolygonBrushTool::HandleMouseDown(const math::Vector2f& world_pos)
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

    if(angle > threashold)
    {
        const math::Vector2f& position = m_polygon->Position();
        m_polygon->AddVertex(m_previousPoint - position);

        m_previouslyAddedPoint = m_previousPoint;
        m_direction = math::INF;
    }

    m_previousPoint = world_pos;
}
