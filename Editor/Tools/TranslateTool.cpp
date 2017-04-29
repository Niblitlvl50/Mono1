
#include "TranslateTool.h"
#include "Entity/IEntity.h"
#include "Math/Vector.h"
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
    math::Vector new_pos = m_beginTranslate - delta + m_positionDiff;
    const std::pair<int, math::Vector>& snapped_position = m_editor->FindSnapPosition(new_pos);

    if(snapped_position.first != -1)
        new_pos = snapped_position.second - new_pos;

    m_entity->SetPosition(new_pos);
    m_editor->UpdateGrabbers();
}
