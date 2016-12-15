//
//  RotateTool.cpp
//  MonoiOS
//
//  Created by Niklas Damberg on 03/07/16.
//
//

#include "RotateTool.h"
#include "Math/MathFunctions.h"
#include "Math/Vector.h"
#include "Math/Quad.h"
#include "IEntity.h"
#include "Editor.h"

using namespace editor;

RotateTool::RotateTool(Editor* editor)
    : m_editor(editor)
{ }

void RotateTool::Begin()
{ }

void RotateTool::End()
{
    m_entity = nullptr;
}

bool RotateTool::IsActive() const
{
    return m_entity != nullptr;
}

void RotateTool::HandleContextMenu(int menu_index)
{ }

void RotateTool::HandleMouseDown(const math::Vector& world_pos, mono::IEntityPtr entity)
{
    m_entity = entity;
    if(!m_entity)
        return;

    const math::Vector& position = m_entity->Position() + m_entity->BasePoint();
    const float rotation = m_entity->Rotation();
    m_rotationDiff = rotation - math::AngleBetweenPoints(position, world_pos);
}

void RotateTool::HandleMouseUp(const math::Vector& world_pos)
{
    End();
}

void RotateTool::HandleMousePosition(const math::Vector& world_pos)
{
    if(!m_entity)
        return;

    const math::Vector& position = m_entity->Position() + m_entity->BasePoint();
    const float angle = math::AngleBetweenPoints(position, world_pos);

    m_entity->SetRotation(angle + m_rotationDiff);
    m_editor->UpdateGrabbers();
}


