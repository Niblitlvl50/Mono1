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
#include "IRenderer.h"

using namespace game;

Meteor::Meteor(float x, float y)
    : mSprite("meteorite.sprite")
{
    mPosition = math::Vector2f(x, y);
    mScale = 20.0f;
    
    mPhysicsObject.body = cm::Factory::CreateBody(15.0f, 1.0f);
    mPhysicsObject.body->SetPosition(mPosition);    

    cm::IShapePtr shape = cm::Factory::CreateShape(mPhysicsObject.body, 10.0f, 10.0f);
    shape->SetElasticity(0.9f);
    
    mPhysicsObject.shapes.push_back(shape);    
}

void Meteor::Draw(mono::IRenderer& renderer) const
{
    renderer.DrawSprite(mSprite);
}

void Meteor::Update(unsigned int delta)
{ }
