//
//  WeaponFactory.cpp
//  MonoOSX
//
//  Created by Niklas Damberg on 19/07/15.
//
//

#include "WeaponFactory.h"
#include "Weapons/Weapon.h"
#include "Math/MathFwd.h"

#include "Explosion.h"
#include "Physics/CMIBody.h"
#include "IPhysicsEntity.h"

#include "EventHandler/EventHandler.h"
#include "Events/DamageEvent.h"
#include "Events/SpawnEntityEvent.h"
#include "Events/RemoveEntityEvent.h"
#include "Events/ShockwaveEvent.h"


namespace
{
    void StandardCollision(const mono::IPhysicsEntity* bullet, const mono::IBodyPtr& other, mono::EventHandler& event_handler)
    {
        event_handler.DispatchEvent(game::DamageEvent(other, 20));
        event_handler.DispatchEvent(game::RemoveEntityEvent(bullet->Id()));
    }

    void RocketCollision(const mono::IPhysicsEntity* bullet, const mono::IBodyPtr& other, mono::EventHandler& event_handler)
    {
        game::ExplosionConfiguration config;
        config.position = bullet->Position();
        config.scale = 60.0f;
        config.rotation = 0.0f;
        config.sprite_file = "sprites/explosion.sprite";

        const game::SpawnEntityEvent event(std::make_shared<game::Explosion>(config, event_handler));
        event_handler.DispatchEvent(event);

        event_handler.DispatchEvent(game::ShockwaveEvent(config.position, 100));
        event_handler.DispatchEvent(game::RemoveEntityEvent(bullet->Id()));
    }

    void CacoPlasmaCollision(const mono::IPhysicsEntity* bullet, const mono::IBodyPtr& other, mono::EventHandler& event_handler)
    {
        game::ExplosionConfiguration config;
        config.position = bullet->Position();
        config.scale = 60.0f;
        config.rotation = 0.0f;
        config.sprite_file = "sprites/cacoexplosion.sprite";

        const game::SpawnEntityEvent event(std::make_shared<game::Explosion>(config, event_handler));
        event_handler.DispatchEvent(event);
        event_handler.DispatchEvent(game::DamageEvent(other, 20));
        event_handler.DispatchEvent(game::RemoveEntityEvent(bullet->Id()));
    }
}

std::unique_ptr<game::IWeaponSystem> game::Factory::CreateWeapon(game::WeaponType weapon, mono::EventHandler& eventHandler)
{
    WeaponConfiguration config;

    switch(weapon)
    {
        case game::WeaponType::STANDARD:
        {
            BulletConfiguration bullet_config;
            bullet_config.life_span = math::INF;
            bullet_config.fuzzy_life_span = 0;
            bullet_config.collision_radius = 2.0f;
            bullet_config.collision_callback = StandardCollision;
            bullet_config.sprite_file = "sprites/firebullet.sprite";
            bullet_config.sound_file = nullptr;

            config.rounds_per_second = 10.0f;
            config.bullet_force = 400.0f;
            config.fire_sound = "sound/laser.wav";
            config.bullet_config = bullet_config;

            break;
        }
        case game::WeaponType::ROCKET:
        {
            BulletConfiguration bullet_config;
            bullet_config.life_span = 0.6f;
            bullet_config.fuzzy_life_span = 0.3f;
            bullet_config.collision_radius = 2.0f;
            bullet_config.collision_callback = RocketCollision;
            bullet_config.sprite_file = "sprites/laser.sprite";
            bullet_config.sound_file = nullptr;

            config.rounds_per_second = 1.5f;
            config.bullet_force = 300.0f;
            config.bullet_config = bullet_config;

            break;
        }
        case game::WeaponType::CACOPLASMA:
        {
            BulletConfiguration bullet_config;
            bullet_config.life_span = 1.0f;
            bullet_config.fuzzy_life_span = 0.3f;
            bullet_config.collision_radius = 5.0f;
            bullet_config.collision_callback = CacoPlasmaCollision;
            bullet_config.sprite_file = "sprites/cacobullet.sprite";
            bullet_config.sound_file = nullptr;

            config.rounds_per_second = 2.0f;
            config.bullet_force = 400.0f;
            config.bullet_config = bullet_config;

            break;
        }
    }

    return std::make_unique<game::Weapon>(config, eventHandler);
}
