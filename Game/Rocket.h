//
//  Rocket.h
//  MonoOSX
//
//  Created by Niklas Damberg on 16/11/14.
//
//

#pragma once

#include "PhysicsEntityBase.h"
#include "Sprite.h"
#include "CMIBody.h"

namespace game
{
    class Rocket :  public mono::PhysicsEntityBase, public cm::ICollisionHandler
    {
    public:
        Rocket(const math::Vector2f& start, float rotation, mono::EventHandler& eventHandler);

        virtual void Draw(mono::IRenderer& renderer) const;
        virtual void Update(unsigned int delta);

        virtual void OnCollideWith(cm::IBodyPtr body);
        virtual void OnPostStep();

        virtual bool RemoveMe() const;

    private:

        mono::Sprite mSprite;
        mono::EventHandler& mEventHandler;
        bool mRemoveMe;

    };
}
