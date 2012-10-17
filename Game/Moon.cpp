//
//  Moon.cpp
//  Mono1
//
//  Created by Niblit on 2012-10-07.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//


#include "Moon.h"
#include "CMIBody.h"
#include "CMIShape.h"
#include "CMFactory.h"

using namespace game;


Moon::Moon(float x, float y, float radius)
    : mSprite("moon.sprite")
{
    mPosition = Math::Vector2f(x, y);
    mScale = radius * 2.0f;
    
    mPhysicsObject.body = cm::Factory::CreateBody(150000.0f, 1.0f);
    mPhysicsObject.body->SetPosition(mPosition);

    cm::IShapePtr shape = cm::Factory::CreateShape(mPhysicsObject.body, radius, Math::Vector2f(0.0f, 0.0f), true);
    shape->SetElasticity(0.9f);
    
    mPhysicsObject.shapes.push_back(shape);
}

void Moon::Draw(mono::IRenderer&) const
{
    mSprite.Draw();
}

void Moon::Update(unsigned int delta)
{
    //mSprite.doUpdate(delta);
}
