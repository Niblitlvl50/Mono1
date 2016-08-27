//
//  Meteor.cpp
//  Mono1
//
//  Created by Niblit on 2013-01-27.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#include "Meteor.h"
#include "IRenderer.h"
#include "CMIBody.h"
#include "CMIShape.h"
#include "CMFactory.h"

#include "EntityProperties.h"
#include "ISprite.h"
#include "SpriteFactory.h"

using namespace game;

Meteor::Meteor(float x, float y)
{
    mSprite = mono::CreateSprite("meteorite.sprite");

    mPosition = math::Vector2f(x, y);
    mScale = math::Vector2f(20.0f, 20.0f);

    SetProperty(EntityProperties::DAMAGABLE);
    
    mPhysicsObject.body = mono::PhysicsFactory::CreateBody(15.0f, 1.0f);
    mPhysicsObject.body->SetPosition(mPosition);    

    mono::IShapePtr shape = mono::PhysicsFactory::CreateShape(mPhysicsObject.body, 10.0f, 10.0f);
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
