//
//  TranslateTool.cpp
//  MonoiOS
//
//  Created by Niklas Damberg on 03/07/16.
//
//

#include "TranslateTool.h"
#include "IEntity.h"
#include "Math/Vector.h"
#include "Math/Quad.h"
#include "Editor.h"

using namespace editor;

TranslateTool::TranslateTool(Editor* editor)
    : m_editor(editor)
{ }

void TranslateTool::Begin()
{ }

void TranslateTool::End()
{
    m_entity = nullptr;
}

bool TranslateTool::IsActive() const
{
    return m_entity != nullptr;
}

void TranslateTool::HandleContextMenu(int menu_index)
{ }

void TranslateTool::HandleMouseDown(const math::Vector& world_pos, mono::IEntityPtr entity)
{
    m_entity = entity;
    if(!m_entity)
        return;
    
    m_beginTranslate = world_pos;
    m_positionDiff = m_entity->Position() - world_pos;
}

void TranslateTool::HandleMouseUp(const math::Vector& world_pos)
{
    End();
}

void TranslateTool::HandleMousePosition(const math::Vector& world_pos)
{
    if(!m_entity)
        return;

    const math::Vector& delta = m_beginTranslate - world_pos;
    const math::Vector& new_pos = m_beginTranslate - delta;

    m_entity->SetPosition(new_pos + m_positionDiff);
    m_editor->UpdateGrabbers();
}


