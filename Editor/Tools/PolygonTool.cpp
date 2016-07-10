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

void PolygonTool::Begin()
{
    m_polygon = std::make_shared<editor::PolygonEntity>();
    m_editor->AddPolygon(m_polygon);
    m_firstPoint = true;
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
}

void PolygonTool::HandleMouseUp(const math::Vector2f& world_pos)
{ }

void PolygonTool::HandleMousePosition(const math::Vector2f& world_pos)
{ }
