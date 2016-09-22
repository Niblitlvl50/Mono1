//
//  Rocket.cpp
//  MonoOSX
//
//  Created by Niklas Damberg on 16/11/14.
//
//

#include "Rocket.h"

#include "Physics/CMIBody.h"
#include "Physics/CMIShape.h"
#include "Physics/CMFactory.h"

#include "IRenderer.h"
#include "Sprite/ISprite.h"
#include "Sprite/SpriteFactory.h"
#include "EventHandler/EventHandler.h"
#include "Events/ShockwaveEvent.h"
#include "Events/SpawnEntityEvent.h"
#include "Events/RemoveEntityEvent.h"
#include "Explosion.h"

using namespace game;

Rocket::Rocket(const math::Vector2f& start, float rotation, mono::EventHandler& eventHandler)
    : mEventHandler(eventHandler)
{
    mScale = math::Vector2f(25.0f, 25.0f);

    mPhysicsObject.body = mono::PhysicsFactory::CreateBody(1.0f, 1.0f);
    mPhysicsObject.body->SetPosition(start);
    mPhysicsObject.body->SetAngle(rotation);
    mPhysicsObject.body->SetCollisionHandler(this);

    mono::IShapePtr shape = mono::PhysicsFactory::CreateShape(mPhysicsObject.body, 7.0f, math::Vector2f(0.0f, 0.0f));

    mPhysicsObject.body->SetMoment(shape->GetInertiaValue());
    mPhysicsObject.shapes.push_back(shape);

    mSprite = mono::CreateSprite("laser.sprite");
}

void Rocket::Draw(mono::IRenderer& renderer) const
{
    renderer.DrawSprite(*mSprite);
}

void Rocket::Update(unsigned int delta)
{
    mSprite->doUpdate(delta);
}

void Rocket::OnCollideWith(const mono::IBodyPtr& body)
{
    const game::SpawnEntityEvent event(std::make_shared<Explosion>(mEventHandler, mPosition, 60));
    mEventHandler.DispatchEvent(event);

    mEventHandler.DispatchEvent(game::ShockwaveEvent(mPosition, 100));
    mEventHandler.DispatchEvent(game::RemoveEntityEvent(Id()));
}

void Rocket::OnPostStep()
{ }
