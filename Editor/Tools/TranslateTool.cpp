//
//  TranslateTool.cpp
//  MonoiOS
//
//  Created by Niklas Damberg on 03/07/16.
//
//

#include "TranslateTool.h"
#include "Editor.h"
#include "MathFunctions.h"
#include "Vector2f.h"
#include "Quad.h"
#include "Polygon.h"

using namespace editor;


TranslateTool::TranslateTool(EditorZone* editor)
    : m_editor(editor)
{ }

void TranslateTool::Begin()
{ }

void TranslateTool::End()
{
    if(m_polygon)
    {
        m_polygon->SetSelected(false);
        m_polygon = nullptr;
    }
}

bool TranslateTool::IsActive() const
{
    return m_polygon != nullptr;
}

void TranslateTool::HandleMouseDown(const math::Vector2f& world_pos)
{
    for(auto& polygon : m_editor->m_polygons)
        polygon->SetSelected(false);

    for(auto& polygon : m_editor->m_polygons)
    {
        const math::Quad& bb = polygon->BoundingBox();
        const bool found_polygon = math::PointInsideQuad(world_pos, bb);
        if(found_polygon)
        {
            m_polygon = polygon;
            m_polygon->SetSelected(true);
            m_beginTranslate = world_pos;
            m_positionDiff = m_polygon->Position() - world_pos;
            break;
        }
    }
}

void TranslateTool::HandleMouseUp(const math::Vector2f& world_pos)
{
    End();
}

void TranslateTool::HandleMousePosition(const math::Vector2f& world_pos)
{
    if(!m_polygon)
        return;

    const math::Vector2f& delta = m_beginTranslate - world_pos;
    const math::Vector2f& new_pos = m_beginTranslate - delta;

    m_polygon->SetPosition(new_pos + m_positionDiff);
}


