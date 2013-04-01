//
//  PhysicsEntityBase.cpp
//  Mono1
//
//  Created by Niblit on 2013-03-31.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#include "PhysicsEntityBase.h"
#include "Quad.h"
#include "SysOpenGL.h"
#include "CMIBody.h"

using namespace mono;

PhysicsEntityBase::PhysicsEntityBase()
    : mRotation(0.0f),
      mScale(1.0f)
{ }

const math::Vector2f& PhysicsEntityBase::Position() const
{
    return mPosition;
}

math::Quad PhysicsEntityBase::BoundingBox() const
{
    const float x = mPosition.mX - (mScale / 2.0f);
    const float y = mPosition.mY - (mScale / 2.0f);
    return math::Quad(x, y, x + mScale, y + mScale);
}

cm::Object& PhysicsEntityBase::GetPhysics()
{
    return mPhysicsObject;
}

void PhysicsEntityBase::doDraw(IRenderer& renderer) const
{
    glTranslatef(mPosition.mX, mPosition.mY, 0.0f);
    glRotatef(mRotation, 0.0f, 0.0f, 1.0f);    
    glScalef(mScale, mScale, mScale);    

    Draw(renderer);
}

void PhysicsEntityBase::doUpdate(unsigned int delta)
{
    mPosition = mPhysicsObject.body->GetPosition();
    mRotation = mPhysicsObject.body->GetAngle();
    
    Update(delta);
}

