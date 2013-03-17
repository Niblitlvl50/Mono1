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

using namespace game;

Meteor::Meteor(float x, float y)
    : mSprite("meteorite.sprite")
{
    mPosition = Math::Vector2f(x, y);
    mScale = 20.0f;
    
    mPhysicsObject.body = cm::Factory::CreateBody(15.0f, 1.0f);
    cm::IShapePtr shape = cm::Factory::CreateShape(mPhysicsObject.body, 10.0f, 10.0f, false);
    shape->SetElasticity(0.9f);
    
    mPhysicsObject.shapes.push_back(shape);    
    mPhysicsObject.body->SetPosition(mPosition);    
}

void Meteor::Draw(mono::IRenderer& renderer) const
{
    mSprite.Draw();
}

void Meteor::Update(unsigned int delta)
{
    mPosition = mPhysicsObject.body->GetPosition();
    mRotation = mPhysicsObject.body->GetAngle();    
}
