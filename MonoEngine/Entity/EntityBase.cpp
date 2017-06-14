
#include "Entity/EntityBase.h"
#include "Utils.h"
#include "Math/Quad.h"
#include "Math/Matrix.h"
#include "Rendering/IRenderer.h"

#include "System/UID.h"

using namespace mono;

EntityBase::EntityBase()
    : m_uid(System::CreateUID()),
      m_properties(0),
      m_scale(1.0f, 1.0f),
      m_rotation(0.0f)
{ }

void EntityBase::doDraw(IRenderer& renderer) const
{
    const math::Matrix& matrix = renderer.GetCurrentTransform() * Transformation();

    for(const auto& child : m_children)
    {
        renderer.PushNewTransform(matrix);
        child->doDraw(renderer);
    }

    renderer.PushNewTransform(matrix);
    Draw(renderer);
}

void EntityBase::doUpdate(unsigned int delta)
{
    for(auto& child : m_children)
        child->doUpdate(delta);

    Update(delta);
}

const math::Vector& EntityBase::Position() const
{
    return m_position;
}

const math::Vector& EntityBase::BasePoint() const
{
    return m_base_point;
}

void EntityBase::SetScale(const math::Vector& scale)
{
    m_scale = scale;
}

float EntityBase::Rotation() const
{
    return m_rotation;
}

const math::Vector& EntityBase::Scale() const
{
    return m_scale;
}

void EntityBase::SetPosition(const math::Vector& position)
{
    m_position = position;
}

void EntityBase::SetBasePoint(const math::Vector& base_point)
{
    m_base_point = base_point;
}

void EntityBase::SetRotation(float rotation)
{
    m_rotation = rotation;
}

math::Quad EntityBase::BoundingBox() const
{
    const math::Vector& halfScale = m_scale / 2.0f;
    math::Quad thisbb(m_position - halfScale, m_position + halfScale);

    for(const auto& child : m_children)
    {
        math::Quad childbb = (child->BoundingBox() * m_scale);
        childbb.mA += m_position;
        childbb.mB += m_position;
        thisbb |= childbb;
    }
            
    return thisbb;
}

math::Matrix EntityBase::Transformation() const
{
    const math::Vector& rotationPoint = m_base_point * m_scale;
    const math::Vector& translate = m_position + rotationPoint;

    math::Matrix translation;
    math::Translate(translation, translate);

    math::Matrix rotation;
    math::RotateZ(rotation, m_rotation);

    math::Matrix translateRotation;
    math::Translate(translateRotation, -rotationPoint);

    math::Matrix scale;
    math::ScaleXY(scale, m_scale);

    math::Matrix matrix;
    matrix *= translation;
    matrix *= rotation;
    matrix *= translateRotation;
    matrix *= scale;

    return matrix;
}

unsigned int EntityBase::Id() const
{
    return m_uid;
}

void EntityBase::SetProperty(unsigned int property)
{
    m_properties |= property;
}

bool EntityBase::HasProperty(unsigned int property) const
{
    return m_properties & property;
}

void EntityBase::AddChild(const IEntityPtr& child)
{
    m_children.push_back(child);
}

void EntityBase::RemoveChild(const IEntityPtr& child)
{
    FindAndRemove(m_children, child);
}

