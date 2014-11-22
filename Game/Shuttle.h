//
//  Shuttle.h
//  Mono1
//
//  Created by Niblit on 2012-08-25.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#pragma once

#include "PhysicsEntityBase.h"
#include "Sprite.h"
#include "ShuttleController.h"
#include "CMIBody.h"

namespace game
{
    class Shuttle : public mono::PhysicsEntityBase, public cm::ICollisionHandler
    {
    public:
        
        Shuttle(float x, float y, mono::EventHandler& eventHandler);

    private:

        friend class ShuttleController;
        
        void ApplyRotationForce(float force);
        void ApplyThrustForce(float force);
        void ApplyImpulse(const math::Vector2f& force);

        void Fire();
        void StartThrusting();
        void StopThrusting();
        
        virtual void Draw(mono::IRenderer& renderer) const;
        virtual void Update(unsigned int delta);
        
        virtual void OnCollideWith(cm::IBodyPtr body);
        virtual void OnPostStep();
        
        mono::Sprite mSprite;
        ShuttleController mController;
        mono::EventHandler& mEventHandler;
    };
}

