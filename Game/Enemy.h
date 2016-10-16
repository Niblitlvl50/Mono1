
#pragma once

#include "MonoFwd.h"
#include "MonoPtrFwd.h"
#include "PhysicsEntityBase.h"
#include "Math/Vector2f.h"
#include "Weapons/IWeaponSystem.h"

namespace game
{
    class Enemy;

    class IEnemyController : public mono::IUpdatable
    {
    public:

        virtual void Initialize(Enemy* enemy) = 0;
    };

    struct EnemySetup
    {
        const char* sprite_file = nullptr;
        float size = 1.0f;
        float mass = 1.0f;
        math::Vector2f position;
        std::unique_ptr<IEnemyController> controller;
    };

    class Enemy : public mono::PhysicsEntityBase
    {
    public:

        Enemy(EnemySetup& setup, mono::EventHandler& event_handler);

        virtual void Draw(mono::IRenderer& renderer) const;
        virtual void Update(unsigned int delta);

    private:

        friend IEnemyController;

        mono::EventHandler& m_eventHandler;
        std::unique_ptr<IEnemyController> m_controller;
        std::unique_ptr<IWeaponSystem> m_weapon;
        mono::ISpritePtr m_sprite;
    };
}
