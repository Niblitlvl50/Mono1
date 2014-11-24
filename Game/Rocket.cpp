//
//  Rocket.cpp
//  MonoOSX
//
//  Created by Niklas Damberg on 16/11/14.
//
//

#include "Rocket.h"

#include "CMIBody.h"
#include "CMIShape.h"
#include "CMFactory.h"

#include "IRenderer.h"
#include "EventHandler.h"
#include "ShockwaveEvent.h"
#include "SpawnEntityEvent.h"
#include "Explosion.h"

using namespace game;

Rocket::Rocket(const math::Vector2f& start, float rotation, mono::EventHandler& eventHandler)
    : mSprite("laser.sprite"),
      mEventHandler(eventHandler),
      mRemoveMe(false)
{
    mScale = 25.0f;

    mPhysicsObject.body = cm::Factory::CreateBody(1.0f, 1.0f);
    mPhysicsObject.body->SetPosition(start);
    mPhysicsObject.body->SetAngle(rotation);
    mPhysicsObject.body->SetCollisionHandler(this);

    cm::IShapePtr shape = cm::Factory::CreateShape(mPhysicsObject.body, 7.0f, math::Vector2f(0.0f, 0.0f));

    mPhysicsObject.body->SetMoment(shape->GetInertiaValue());
    mPhysicsObject.shapes.push_back(shape);
}

void Rocket::Draw(mono::IRenderer& renderer) const
{
    renderer.DrawSprite(mSprite);
}

void Rocket::Update(unsigned int delta)
{
    mSprite.doUpdate(delta);
}

void Rocket::OnCollideWith(cm::IBodyPtr body)
{
    const game::SpawnEntityEvent event(std::make_shared<Explosion>(mPosition, 60));
    mEventHandler.DispatchEvent(event);

    mEventHandler.DispatchEvent(ShockwaveEvent(mPosition, 100));
    mRemoveMe = true;
}

void Rocket::OnPostStep()
{ }

bool Rocket::RemoveMe() const
{
    return mRemoveMe;
}
