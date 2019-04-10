
#include "Entity/EntityBase.h"
#include "Util/Algorithm.h"
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

void EntityBase::doUpdate(uint32_t delta_ms)
{
    for(auto& child : m_children)
        child->doUpdate(delta_ms);

    Update(delta_ms);
}

const math::Vector& EntityBase::Position() const
{
    return m_position;
}

const math::Vector& EntityBase::BasePoint() const
{
    return m_pivot_point;
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

void EntityBase::SetBasePoint(const math::Vector& pivot_point)
{
    m_pivot_point = pivot_point;
}

void EntityBase::SetRotation(float rotation)
{
    m_rotation = rotation;
}

math::Quad EntityBase::BoundingBox() const
{
//    math::Quad thisbb(math::INF, math::INF, -math::INF, -math::INF);
//
//    const math::Matrix& local_to_world = Transformation();
//    thisbb |= math::Transform(local_to_world, math::Vector(-0.5f, -0.5f));
//    thisbb |= math::Transform(local_to_world, math::Vector(-0.5f,  0.5f));
//    thisbb |= math::Transform(local_to_world, math::Vector( 0.5f,  0.5f));
//    thisbb |= math::Transform(local_to_world, math::Vector( 0.5f, -0.5f));

    const math::Vector& half_scale = m_scale / 2.0f;
    math::Quad thisbb = math::Quad(m_position - half_scale, m_position + half_scale);

    if(!m_children.empty())    
    {
        const math::Matrix& local_to_world = Transformation();
        for(const auto& child : m_children)
            thisbb |= math::Transform(local_to_world, child->BoundingBox());
    }
            
    return thisbb;
}

math::Matrix EntityBase::Transformation() const
{
    const math::Vector& pivot_point = m_pivot_point * m_scale;
    const math::Vector& translate = m_position + pivot_point;

    math::Matrix translation;
    math::Translate(translation, translate);

    const math::Matrix rotation = math::CreateMatrixFromZRotation(m_rotation);

    math::Matrix pivot_offset;
    math::Translate(pivot_offset, -pivot_point);

    math::Matrix scale;
    math::ScaleXY(scale, m_scale);

    return translation * rotation * pivot_offset * scale;
}

uint32_t EntityBase::Id() const
{
    return m_uid;
}

void EntityBase::SetId(uint32_t new_id)
{
    m_uid = new_id;
}

void EntityBase::SetProperty(uint32_t property)
{
    m_properties |= property;
}

bool EntityBase::HasProperty(uint32_t property) const
{
    return m_properties & property;
}

void EntityBase::AddChild(const IEntityPtr& child)
{
    m_children.push_back(child);
}

void EntityBase::RemoveChild(const IEntityPtr& child)
{
    mono::remove(m_children, child);
}

