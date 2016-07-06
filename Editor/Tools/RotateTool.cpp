//
//  RotateTool.cpp
//  MonoiOS
//
//  Created by Niklas Damberg on 03/07/16.
//
//

#include "RotateTool.h"
#include "Editor.h"
#include "MathFunctions.h"
#include "Vector2f.h"
#include "Quad.h"
#include "Polygon.h"

using namespace editor;


RotateTool::RotateTool(EditorZone* editor)
    : m_editor(editor)
{ }

void RotateTool::Begin()
{ }

void RotateTool::End()
{
    if(m_polygon)
    {
        m_polygon->m_selected = false;
        m_polygon = nullptr;
    }
}

bool RotateTool::IsActive() const
{
    return m_polygon != nullptr;
}

void RotateTool::HandleMouseDown(const math::Vector2f& world_pos)
{
    for(auto& polygon : m_editor->m_polygons)
        polygon->m_selected = false;

    for(auto& polygon : m_editor->m_polygons)
    {
        const math::Quad& bb = polygon->BoundingBox();
        const bool found_polygon = math::PointInsideQuad(world_pos, bb);
        if(found_polygon)
        {
            m_polygon = polygon;
            m_polygon->m_selected = true;

            const math::Vector2f& position = m_polygon->Position();
            const float rotation = m_polygon->Rotation();
            m_rotationDiff = rotation - math::AngleBetweenPoints(position, world_pos);

            break;
        }
    }
}

void RotateTool::HandleMouseUp(const math::Vector2f& world_pos)
{
    End();
}

void RotateTool::HandleMousePosition(const math::Vector2f& world_pos)
{
    if(!m_polygon)
        return;

    const math::Vector2f& position = m_polygon->Position();
    const float angle = math::AngleBetweenPoints(position, world_pos);

    m_polygon->SetRotation(angle + m_rotationDiff);
}


