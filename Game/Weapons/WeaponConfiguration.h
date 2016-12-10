
#pragma once

#include "MonoFwd.h"
#include "MonoPtrFwd.h"
#include "Physics/CMFwd.h"

namespace game
{
    using bullet_impact_callback = void (*)(const mono::IPhysicsEntity*, const mono::IBodyPtr&, mono::EventHandler&);
    

    struct BulletConfiguration
    {
        float life_span = 1.0f;
        float fuzzy_life_span = 0.0f;
        float collision_radius = 1.0f;
        bullet_impact_callback collision_callback = nullptr;

        const char* sprite_file = nullptr;
        const char* sound_file = nullptr;
    };

    struct WeaponConfiguration
    {
        float rounds_per_second = 1.0f;
        float bullet_force = 100.0f;
        const char* fire_sound = nullptr;

        BulletConfiguration bullet_config;
    };
}
