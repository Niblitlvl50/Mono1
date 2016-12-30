
#pragma once

#include "System/SysTypes.h"
#include "MonoFwd.h"
#include "MonoPtrFwd.h"
#include "Physics/CMFwd.h"
#include "CollisionConfiguration.h"

#include <functional>

namespace game
{
    using BulletImpactCallback = std::function<void (const mono::IPhysicsEntity*, const mono::IBodyPtr&)>;

    struct BulletConfiguration
    {
        float life_span = 1.0f;
        float fuzzy_life_span = 0.0f;
        float collision_radius = 1.0f;
        float scale = 1.0f;
        CollisionCategory collision_category = CollisionCategory::NONE;
        uint collision_mask = 0;
        BulletImpactCallback collision_callback;

        const char* sprite_file = nullptr;
        const char* sound_file = nullptr;
    };

    struct WeaponConfiguration
    {
        float rounds_per_second = 1.0f;
        float fire_rate_multiplier = 1.0f;
        float max_fire_rate = 1.0f;
        float bullet_force = 100.0f;
        const char* fire_sound = nullptr;

        BulletConfiguration bullet_config;
    };
}
