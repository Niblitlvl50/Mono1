
#include "PhysicsEntityBase.h"
#include "Math/Quad.h"
#include "Math/Matrix.h"
#include "Physics/IBody.h"
#include "Rendering/IRenderer.h"
#include "System/SysUID.h"
#include "Utils.h"

using namespace mono;

PhysicsEntityBase::PhysicsEntityBase()
    : m_uid(system::CreateUID()),
      m_properties(0),
      mScale(1.0f, 1.0f),
      mRotation(0.0f)
{ }

const math::Vector& PhysicsEntityBase::Position() const
{
    return mPosition;
}

const math::Vector& PhysicsEntityBase::BasePoint() const
{
    return math::zeroVec;
}

float PhysicsEntityBase::Rotation() const
{
    return mRotation;
}

const math::Vector& PhysicsEntityBase::Scale() const
{
    return mScale;
}

void PhysicsEntityBase::SetPosition(const math::Vector& position)
{
    mPosition = position;
    mPhysicsObject.body->SetPosition(position);
}

void PhysicsEntityBase::SetRotation(float rotation)
{
    mRotation = rotation;
    mPhysicsObject.body->SetAngle(rotation);
}

void PhysicsEntityBase::SetScale(const math::Vector& scale)
{
    mScale = scale;
}

math::Quad PhysicsEntityBase::BoundingBox() const
{
    const math::Vector& halfScale = mScale / 2.0f;
    math::Quad thisbb(mPosition - halfScale, mPosition + halfScale);

    for(const auto& child : m_children)
    {
        math::Quad childbb = (child->BoundingBox() * mScale);
        childbb.mA += mPosition;
        childbb.mB += mPosition;
        thisbb |= childbb;
    }
            
    return thisbb;
}

mono::PhysicsData& PhysicsEntityBase::GetPhysics()
{
    return mPhysicsObject;
}

unsigned int PhysicsEntityBase::Id() const
{
    return m_uid;
}

void PhysicsEntityBase::SetProperty(unsigned int property)
{
    m_properties |= property;
}

bool PhysicsEntityBase::HasProperty(unsigned int property) const
{
    return m_properties & property;
}

void PhysicsEntityBase::doDraw(IRenderer& renderer) const
{
    const math::Matrix& transform = renderer.GetCurrentTransform() * Transformation();

    for(const auto& child : m_children)
    {
        renderer.PushNewTransform(transform);
        child->doDraw(renderer);
    }

    renderer.PushNewTransform(transform);

    Draw(renderer);
}

void PhysicsEntityBase::doUpdate(unsigned int delta)
{
    mPosition = mPhysicsObject.body->GetPosition();
    mRotation = mPhysicsObject.body->GetAngle();

    for(auto& child : m_children)
        child->doUpdate(delta);

    Update(delta);
}

math::Matrix PhysicsEntityBase::Transformation() const
{
    math::Matrix translation;
    math::Translate(translation, mPosition);

    math::Matrix rotation;
    math::RotateZ(rotation, mRotation);

    math::Matrix scale;
    math::ScaleXY(scale, mScale);

    math::Matrix transform;
    transform *= translation;
    transform *= rotation;
    transform *= scale;

    return transform;
}

void PhysicsEntityBase::AddChild(const IEntityPtr& child)
{
    m_children.push_back(child);
}

void PhysicsEntityBase::RemoveChild(const IEntityPtr& child)
{
    FindAndRemove(m_children, child);
}

