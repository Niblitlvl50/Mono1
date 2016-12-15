//
//  Meteor.cpp
//  Mono1
//
//  Created by Niblit on 2013-01-27.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#include "Meteor.h"
#include "IRenderer.h"
#include "Physics/CMIBody.h"
#include "Physics/CMIShape.h"
#include "Physics/CMFactory.h"

#include "EntityProperties.h"
#include "Sprite/ISprite.h"
#include "Sprite/SpriteFactory.h"

using namespace game;

Meteor::Meteor(float x, float y)
{
    mSprite = mono::CreateSprite("sprites/meteorite.sprite");

    mPosition = math::Vector(x, y);
    mScale = math::Vector(20.0f, 20.0f);

    SetProperty(EntityProperties::DAMAGABLE);
    
    mPhysicsObject.body = mono::PhysicsFactory::CreateBody(15.0f, 1.0f);
    mPhysicsObject.body->SetPosition(mPosition);

    mono::IShapePtr shape = mono::PhysicsFactory::CreateShape(mPhysicsObject.body, 10.0f, math::zeroVec);
    shape->SetElasticity(0.9f);
    
    mPhysicsObject.body->SetMoment(shape->GetInertiaValue());
    mPhysicsObject.shapes.push_back(shape);
}

void Meteor::Draw(mono::IRenderer& renderer) const
{
    renderer.DrawSprite(*mSprite);
}

void Meteor::Update(unsigned int delta)
{ }
