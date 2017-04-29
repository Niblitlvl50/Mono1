
#include "Meteor.h"
#include "Rendering/IRenderer.h"
#include "Physics/IBody.h"
#include "Physics/IShape.h"
#include "Physics/CMFactory.h"

#include "EntityProperties.h"
#include "Sprite/ISprite.h"
#include "Sprite/SpriteFactory.h"

using namespace game;

Meteor::Meteor(float x, float y)
{
    mSprite = mono::CreateSprite("sprites/meteorite.sprite");

    mPosition = math::Vector(x, y);
    mScale = math::Vector(1.0f, 1.0f);

    SetProperty(EntityProperties::DAMAGABLE);
    
    mPhysicsObject.body = mono::PhysicsFactory::CreateBody(15.0f, 1.0f);
    mPhysicsObject.body->SetPosition(mPosition);

    mono::IShapePtr shape = mono::PhysicsFactory::CreateShape(mPhysicsObject.body, mScale.x * 0.5, math::zeroVec);
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
