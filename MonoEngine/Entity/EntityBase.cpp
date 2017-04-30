
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
      mScale(1.0f, 1.0f),
      mRotation(0.0f)
{ }

void EntityBase::doDraw(IRenderer& renderer) const
{
    const math::Matrix& matrix = renderer.GetCurrentTransform() * Transformation();

    for(const auto& child : mChildren)
    {
        renderer.PushNewTransform(matrix);
        child->doDraw(renderer);
    }

    renderer.PushNewTransform(matrix);
    Draw(renderer);
}

void EntityBase::doUpdate(unsigned int delta)
{
    for(auto& child : mChildren)
        child->doUpdate(delta);

    Update(delta);
}

const math::Vector& EntityBase::Position() const
{
    return mPosition;
}

const math::Vector& EntityBase::BasePoint() const
{
    return mBasePoint;
}

void EntityBase::SetScale(const math::Vector& scale)
{
    mScale = scale;
}

float EntityBase::Rotation() const
{
    return mRotation;
}

const math::Vector& EntityBase::Scale() const
{
    return mScale;
}

void EntityBase::SetPosition(const math::Vector& position)
{
    mPosition = position;
}

void EntityBase::SetBasePoint(const math::Vector& base_point)
{
    mBasePoint = base_point;
}

void EntityBase::SetRotation(float rotation)
{
    mRotation = rotation;
}

math::Quad EntityBase::BoundingBox() const
{
    const math::Vector& halfScale = mScale / 2.0f;
    math::Quad thisbb(mPosition - halfScale, mPosition + halfScale);

    for(const auto& child : mChildren)
    {
        math::Quad childbb = (child->BoundingBox() * mScale);
        childbb.mA += mPosition;
        childbb.mB += mPosition;
        thisbb |= childbb;
    }
            
    return thisbb;
}

math::Matrix EntityBase::Transformation() const
{
    const math::Vector& rotationPoint = mBasePoint * mScale;
    const math::Vector& translate = mPosition + rotationPoint;

    math::Matrix translation;
    math::Translate(translation, translate);

    math::Matrix rotation;
    math::RotateZ(rotation, mRotation);

    math::Matrix translateRotation;
    math::Translate(translateRotation, -rotationPoint);

    math::Matrix scale;
    math::ScaleXY(scale, mScale);

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
    mChildren.push_back(child);
}

void EntityBase::RemoveChild(const IEntityPtr& child)
{
    FindAndRemove(mChildren, child);
}

