//
//  RotateTool.cpp
//  MonoiOS
//
//  Created by Niklas Damberg on 03/07/16.
//
//

#include "RotateTool.h"
#include "Math/MathFunctions.h"
#include "Math/Vector2f.h"
#include "Math/Quad.h"
#include "IEntity.h"

using namespace editor;

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

void RotateTool::HandleMouseDown(const math::Vector2f& world_pos, mono::IEntityPtr entity)
{
    m_entity = entity;
    if(!m_entity)
        return;

    const math::Vector2f& position = m_entity->Position() + m_entity->BasePoint();
    const float rotation = m_entity->Rotation();
    m_rotationDiff = rotation - math::AngleBetweenPoints(position, world_pos);
}

void RotateTool::HandleMouseUp(const math::Vector2f& world_pos)
{
    End();
}

void RotateTool::HandleMousePosition(const math::Vector2f& world_pos)
{
    if(!m_entity)
        return;

    const math::Vector2f& position = m_entity->Position() + m_entity->BasePoint();
    const float angle = math::AngleBetweenPoints(position, world_pos);

    m_entity->SetRotation(angle + m_rotationDiff);
}


