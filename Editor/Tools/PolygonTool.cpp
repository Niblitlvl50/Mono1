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

using namespace editor;

PolygonTool::PolygonTool(EditorZone* editor)
    : m_editor(editor)
{ }

void PolygonTool::Start()
{
    m_polygon = std::make_shared<editor::PolygonEntity>();
    m_editor->AddPolygon(m_polygon);
}

void PolygonTool::End()
{
    m_polygon = nullptr;
}

bool PolygonTool::IsActive() const
{
    return m_polygon != nullptr;
}

void PolygonTool::HandleMouseDown(const math::Vector2f& world_pos)
{
    if(!m_polygon)
        return;

    m_polygon->m_points.push_back(world_pos);
}

void PolygonTool::HandleMouseUp(const math::Vector2f& world_pos)
{ }

void PolygonTool::HandleMousePosition(const math::Vector2f& world_pos)
{ }
