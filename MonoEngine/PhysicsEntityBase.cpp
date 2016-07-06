//
//  PhysicsEntityBase.cpp
//  Mono1
//
//  Created by Niblit on 2013-03-31.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#include "PhysicsEntityBase.h"
#include "Quad.h"
#include "Matrix.h"
#include "CMIBody.h"
#include "IRenderer.h"
#include "SysUID.h"

using namespace mono;

PhysicsEntityBase::PhysicsEntityBase()
    : m_uid(system::CreateUID()),
      m_properties(0),
      mRotation(0.0f),
      mScale(1.0f, 1.0f)
{ }

const math::Vector2f PhysicsEntityBase::Position() const
{
    return mPosition;
}

float PhysicsEntityBase::Rotation() const
{
    return mRotation;
}

void PhysicsEntityBase::SetPosition(const math::Vector2f& position)
{
    mPosition = position;
    mPhysicsObject.body->SetPosition(position);
}

void PhysicsEntityBase::SetRotation(float rotation)
{
    mRotation = rotation;
    mPhysicsObject.body->SetAngle(rotation);
}

void PhysicsEntityBase::SetScale(const math::Vector2f& scale)
{
    mScale = scale;
}

math::Quad PhysicsEntityBase::BoundingBox() const
{
    const math::Vector2f& halfScale = mScale / 2.0f;
    return math::Quad(mPosition - halfScale, mPosition + halfScale);
}

mono::Object& PhysicsEntityBase::GetPhysics()
{
    return mPhysicsObject;
}

uint PhysicsEntityBase::Id() const
{
    return m_uid;
}

void PhysicsEntityBase::SetProperty(uint property)
{
    m_properties |= property;
}

bool PhysicsEntityBase::HasProperty(uint property) const
{
    return m_properties & property;
}

void PhysicsEntityBase::doDraw(IRenderer& renderer) const
{
    math::Matrix translation;
    math::Translate(translation, mPosition);

    math::Matrix rotation;
    math::RotateZ(rotation, mRotation);

    math::Matrix scale;
    math::ScaleXY(scale, mScale);

    math::Matrix transform = renderer.GetCurrentTransform();
    transform *= translation;
    transform *= rotation;
    transform *= scale;

    renderer.PushNewTransform(transform);

    Draw(renderer);
}

void PhysicsEntityBase::doUpdate(unsigned int delta)
{
    mPosition = mPhysicsObject.body->GetPosition();
    mRotation = mPhysicsObject.body->GetAngle();
    
    Update(delta);
}

