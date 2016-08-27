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
#include "ISprite.h"
#include "SpriteFactory.h"

using namespace game;


Moon::Moon(float x, float y, float radius)
{
    mSprite = mono::CreateSprite("moon.sprite");
    mPosition = math::Vector2f(x, y);
    mScale = math::Vector2f(radius, radius) * 2.0f;
    
    mPhysicsObject.body = mono::PhysicsFactory::CreateStaticBody();
    mPhysicsObject.body->SetPosition(mPosition);

    mono::IShapePtr shape = mono::PhysicsFactory::CreateShape(mPhysicsObject.body, radius, math::zeroVec);
    shape->SetElasticity(0.9f);
    
    mPhysicsObject.shapes.push_back(shape);
}

void Moon::Draw(mono::IRenderer& renderer) const
{
    renderer.DrawSprite(*mSprite);
}

void Moon::Update(unsigned int delta)
{ }
