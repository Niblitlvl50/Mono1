
#include "WeaponFactory.h"
#include "Weapons/Weapon.h"
#include "Math/MathFwd.h"

#include "Explosion.h"
#include "Physics/IBody.h"
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
        event_handler.DispatchEvent(game::DamageEvent(other, 10));
        event_handler.DispatchEvent(game::RemoveEntityEvent(bullet->Id()));
    }

    void RocketCollision(const mono::IPhysicsEntity* bullet, const mono::IBodyPtr& other, mono::EventHandler& event_handler)
    {
        game::ExplosionConfiguration explosion_config;
        explosion_config.position = bullet->Position();
        explosion_config.scale = 2.0f;
        explosion_config.rotation = 0.0f;
        explosion_config.sprite_file = "sprites/explosion.sprite";

        const game::SpawnEntityEvent event(std::make_shared<game::Explosion>(explosion_config, event_handler));
        event_handler.DispatchEvent(event);
        event_handler.DispatchEvent(game::DamageEvent(other, 20));
        event_handler.DispatchEvent(game::ShockwaveEvent(explosion_config.position, 100));
        event_handler.DispatchEvent(game::RemoveEntityEvent(bullet->Id()));
    }

    void CacoPlasmaCollision(const mono::IPhysicsEntity* bullet, const mono::IBodyPtr& other, mono::EventHandler& event_handler)
    {
        game::ExplosionConfiguration explosion_config;
        explosion_config.position = bullet->Position();
        explosion_config.scale = 2.0f;
        explosion_config.rotation = 0.0f;
        explosion_config.sprite_file = "sprites/cacoexplosion.sprite";

        const game::SpawnEntityEvent event(std::make_shared<game::Explosion>(explosion_config, event_handler));
        event_handler.DispatchEvent(event);
        event_handler.DispatchEvent(game::DamageEvent(other, 20));
        event_handler.DispatchEvent(game::RemoveEntityEvent(bullet->Id()));
    }
}

using namespace game;

WeaponFactory::WeaponFactory(mono::EventHandler& eventHandler)
    : m_eventHandler(eventHandler)
{ }

std::unique_ptr<game::IWeaponSystem> WeaponFactory::CreateWeapon(WeaponType weapon, WeaponFaction faction)
{
    using namespace std::placeholders;

    WeaponConfiguration weapon_config;
    BulletConfiguration& bullet_config = weapon_config.bullet_config;

    const bool enemy_weapon = (faction == WeaponFaction::ENEMY);
    bullet_config.collision_category = enemy_weapon ? CollisionCategory::ENEMY_BULLET : CollisionCategory::PLAYER_BULLET;
    bullet_config.collision_mask = enemy_weapon ? ENEMY_BULLET_MASK : PLAYER_BULLET_MASK;

    switch(weapon)
    {
        case game::WeaponType::STANDARD:
        {
            bullet_config.life_span = 10.0f;
            bullet_config.fuzzy_life_span = 0;
            bullet_config.collision_radius = 0.4f;
            bullet_config.scale = 1.0;
            bullet_config.collision_callback = std::bind(StandardCollision, _1, _2, std::ref(m_eventHandler));
            bullet_config.sprite_file = "sprites/firebullet.sprite";
            bullet_config.sound_file = nullptr;

            weapon_config.rounds_per_second = 6.0f;
            weapon_config.fire_rate_multiplier = 1.1f;
            weapon_config.max_fire_rate = 3.0f;
            weapon_config.bullet_force = 20.0f;
            weapon_config.fire_sound = "sound/laser.wav";

            break;
        }

        case game::WeaponType::ROCKET:
        {
            bullet_config.life_span = 0.2f;
            bullet_config.fuzzy_life_span = 0.3f;
            bullet_config.collision_radius = 0.5f;
            bullet_config.collision_callback = std::bind(RocketCollision, _1, _2, std::ref(m_eventHandler));
            bullet_config.sprite_file = "sprites/laser.sprite";
            bullet_config.sound_file = nullptr;

            weapon_config.rounds_per_second = 1.5f;
            weapon_config.bullet_force = 20.0f;

            break;
        }

        case game::WeaponType::CACOPLASMA:
        {
            bullet_config.life_span = 1.0f;
            bullet_config.fuzzy_life_span = 0.3f;
            bullet_config.collision_radius = 0.5f;
            bullet_config.collision_callback = std::bind(CacoPlasmaCollision, _1, _2, std::ref(m_eventHandler));
            bullet_config.sprite_file = "sprites/cacobullet.sprite";
            bullet_config.scale = 0.5;
            bullet_config.sound_file = nullptr;

            weapon_config.rounds_per_second = 5.0f;
            weapon_config.bullet_force = 20.0f;

            break;
        }

        case game::WeaponType::GENERIC:
        {
            bullet_config.life_span = 10.0f;
            bullet_config.fuzzy_life_span = 0;
            bullet_config.scale = 0.3;
            bullet_config.collision_radius = 0.15f;
            bullet_config.collision_callback = std::bind(StandardCollision, _1, _2, std::ref(m_eventHandler));
            bullet_config.shade = mono::Color::RGBA(1.0f, 0.0f, 0.0f, 1.0f);
            bullet_config.sprite_file = "sprites/generic_bullet.sprite";
            bullet_config.sound_file = nullptr;

            weapon_config.rounds_per_second = 4.0f;
            weapon_config.bullet_force = 15.0f;

            break;
        }
    }

    return std::make_unique<game::Weapon>(weapon_config, m_eventHandler);
}
