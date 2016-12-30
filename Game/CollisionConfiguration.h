
#pragma once

namespace game
{
    enum CollisionCategory
    {
        NONE = 0,
        PLAYER = 1,
        PLAYER_BULLET = 2,
        ENEMY = 4,
        ENEMY_BULLET = 8,
        STATIC = 16
    };

    constexpr unsigned int PLAYER_MASK =
        CollisionCategory::ENEMY |
        CollisionCategory::ENEMY_BULLET |
        CollisionCategory::STATIC;

    constexpr unsigned int PLAYER_BULLET_MASK =
        CollisionCategory::ENEMY |
        CollisionCategory::STATIC;

    constexpr unsigned int ENEMY_MASK =
        CollisionCategory::PLAYER |
        CollisionCategory::PLAYER_BULLET |
        CollisionCategory::ENEMY |
        CollisionCategory::STATIC;

    constexpr unsigned int ENEMY_BULLET_MASK =
        CollisionCategory::PLAYER |
        CollisionCategory::STATIC;

    constexpr unsigned int STATIC_MASK =
        CollisionCategory::PLAYER |
        CollisionCategory::PLAYER_BULLET |
        CollisionCategory::ENEMY |
        CollisionCategory::ENEMY_BULLET;
}
