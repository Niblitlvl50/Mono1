//
//  Shuttle.cpp
//  Mono1
//
//  Created by Niblit on 2012-08-25.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "Shuttle.h"
#include "CMIBody.h"
#include "CMIShape.h"
#include "CMFactory.h"

using namespace game;


Shuttle::Shuttle(float x, float y)
    : mSprite("Invader.sprite")
{
    mPosition = Math::Vector2f(x, y);
    mScale = 20.0f;
    
    mPhysicsObject.body = cm::Factory::CreateBody(15.0f, 1.0f);
    cm::IShapePtr shape = cm::Factory::CreateShape(mPhysicsObject.body, 5.0f, Math::Vector2f(0.0f, 0.0f), false);
    shape->SetElasticity(0.9f);
    
    mPhysicsObject.shapes.push_back(shape);
    
    mPhysicsObject.body->SetPosition(mPosition);
}

void Shuttle::Draw(mono::IRenderer&) const
{
    mSprite.Draw();
}

void Shuttle::Update(unsigned int delta)
{
    mPosition = mPhysicsObject.body->GetPosition();
    mRotation = mPhysicsObject.body->GetAngle();
}


