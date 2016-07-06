/*
 *  DrawableBase.cpp
 *  Monolith1
 *
 *  Created by Niblit on 2011-02-21.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "EntityBase.h"
#include "Utils.h"
#include "Quad.h"
#include "Matrix.h"
#include "IRenderer.h"

#include "SysUID.h"


using namespace mono;

EntityBase::EntityBase()
    : m_uid(system::CreateUID()),
      m_properties(0),
      mRotation(0.0f),
      mScale(1.0f, 1.0f)
{ }

void EntityBase::doDraw(IRenderer& renderer) const
{
    //math::Matrix matrix = m_transform * renderer.GetCurrentTransform();

    const math::Vector2f& rotationPoint = mBasePoint * mScale;
    const math::Vector2f& translate = mPosition + rotationPoint;

    math::Matrix matrix = renderer.GetCurrentTransform();

    math::Matrix translation;
    math::Translate(translation, translate);
    matrix *= translation;

    math::Matrix rotation;
    math::RotateZ(rotation, mRotation);
    matrix *= rotation;

    math::Matrix translateRotation;
    math::Translate(translateRotation, -rotationPoint);
    matrix *= translateRotation;

    math::Matrix scale;
    math::ScaleXY(scale, mScale);
    matrix *= scale;

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
    // Updadate children
    for(auto& child : mChildren)
        child->doUpdate(delta);

    // Update this object
    Update(delta);
}

const math::Vector2f EntityBase::Position() const
{
    //return math::GetPosition(m_transform);
    return mPosition;
}

void EntityBase::SetScale(const math::Vector2f& scale)
{
    //math::ScaleXY(m_transform, scale);
    mScale = scale;
}

float EntityBase::Rotation() const
{
    //return math::GetZRotation(m_transform);
    return mRotation;
}

void EntityBase::SetPosition(const math::Vector2f& position)
{
    //math::Position(m_transform, position);
    mPosition = position;
}

void EntityBase::SetRotation(float rotation)
{
    //math::RotateZ(m_transform, rotation);
    mRotation = rotation;
}

math::Quad EntityBase::BoundingBox() const
{
    //return math::Quad(-10, -10, 10, 10);

    const math::Vector2f& halfScale = mScale / 2.0f;
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

uint EntityBase::Id() const
{
    return m_uid;
}

void EntityBase::SetProperty(uint property)
{
    m_properties |= property;
}

bool EntityBase::HasProperty(uint property) const
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

