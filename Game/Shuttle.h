//
//  Shuttle.h
//  Mono1
//
//  Created by Niblit on 2012-08-25.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#pragma once

#include "MonoPtrFwd.h"
#include "PhysicsEntityBase.h"
#include "ShuttleController.h"
#include "Physics/CMIBody.h"

#include "Weapons/IWeaponSystem.h"

class SpriteEntity;

namespace game
{
    class IWeaponSystem;
    enum class WeaponType;

    enum class BoosterPosition
    {
        LEFT,
        RIGHT,
        MAIN,
        ALL
    };

    class Shuttle : public mono::PhysicsEntityBase, public mono::ICollisionHandler
    {
    public:
        
        Shuttle(const math::Vector& postiion, mono::EventHandler& eventHandler);

    private:

        friend class ShuttleController;

        void SelectWeapon(WeaponType weapon);
        
        void ApplyRotationForce(float force);
        void ApplyThrustForce(float force);
        void ApplyImpulse(const math::Vector& force);

        void Fire();
        void StopFire();

        void SetBoosterThrusting(BoosterPosition position, bool enabled);
        
        virtual void Draw(mono::IRenderer& renderer) const;
        virtual void Update(unsigned int delta);
        
        virtual void OnCollideWith(const mono::IBodyPtr& body);
        virtual void OnPostStep();

        std::unique_ptr<IWeaponSystem> m_weapon;

        mono::ISpritePtr m_sprite;
        ShuttleController m_controller;
        mono::EventHandler& m_event_handler;

        bool m_fire;

        std::shared_ptr<SpriteEntity> m_left_booster;
        std::shared_ptr<SpriteEntity> m_right_booster;
    };
}

