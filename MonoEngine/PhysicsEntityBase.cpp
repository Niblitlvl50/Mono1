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

using namespace mono;

PhysicsEntityBase::PhysicsEntityBase()
    : mRotation(0.0f),
      mScale(1.0f, 1.0f)
{ }

const math::Vector2f& PhysicsEntityBase::Position() const
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

math::Quad PhysicsEntityBase::BoundingBox() const
{
    const math::Vector2f& halfScale = mScale / 2.0f;
    return math::Quad(mPosition - halfScale, mPosition + halfScale);
}

cm::Object& PhysicsEntityBase::GetPhysics()
{
    return mPhysicsObject;
}

bool PhysicsEntityBase::RemoveMe() const
{
    return false;
}

void PhysicsEntityBase::doDraw(IRenderer& renderer) const
{
    math::Matrix transform = renderer.GetCurrentTransform();

    math::Matrix translation;
    math::Translate(translation, mPosition);

    math::Matrix rotation;
    math::RotateZ(rotation, mRotation);

    math::Matrix scale;
    math::ScaleXY(scale, mScale);

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

