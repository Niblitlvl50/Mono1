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
#include "IAction.h"
#include "IRenderer.h"

#include "SysUID.h"

#include <functional>

using namespace mono;

EntityBase::EntityBase()
    : m_uid(system::CreateUID()),
      m_properties(0),
      mRotation(0.0f),
      mScale(1.0f, 1.0f)
{ }

void EntityBase::doDraw(IRenderer& renderer) const
{
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
    
    // Update actions
    for(auto& action : mActions)
        action->Update(delta);
    
    // Check if any actions has finished and remove them if that is the case
    const auto removeFunc = std::bind(&IAction::Finished, std::placeholders::_1);
    const auto it = std::remove_if(mActions.begin(), mActions.end(), removeFunc);
    if(it != mActions.end())
        mActions.erase(it, mActions.end());
    
    // Update this object
    Update(delta);
}

const math::Vector2f& EntityBase::Position() const
{
    return mPosition;
}

float EntityBase::Rotation() const
{
    return mRotation;
}

void EntityBase::SetPosition(const math::Vector2f& position)
{
    mPosition = position;
}

void EntityBase::SetRotation(float rotation)
{
    mRotation = rotation;
}

math::Quad EntityBase::BoundingBox() const
{
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

void EntityBase::AddAction(const IActionPtr& action)
{
    mActions.push_back(action);
}

