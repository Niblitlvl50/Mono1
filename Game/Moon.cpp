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
#include "IRenderer.h"

using namespace game;


Moon::Moon(float x, float y, float radius)
    : mSprite("moon.sprite")
{
    mPosition = math::Vector2f(x, y);
    mScale = radius * 2.0f;
    
    mPhysicsObject.body = cm::Factory::CreateStaticBody();
    mPhysicsObject.body->SetPosition(mPosition);

    cm::IShapePtr shape = cm::Factory::CreateShape(mPhysicsObject.body, radius, math::Vector2f(0.0f, 0.0f));
    shape->SetElasticity(0.9f);
    
    mPhysicsObject.shapes.push_back(shape);
}

void Moon::Draw(mono::IRenderer& renderer) const
{
    renderer.DrawSprite(mSprite);
}

void Moon::Update(unsigned int delta)
{ }
