
#pragma once

#include "Rendering/RenderPtrFwd.h"
#include "Entity/PhysicsEntityBase.h"
#include "Math/Vector.h"

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
        math::Vector position;
        std::unique_ptr<IEnemyController> controller;
    };

    class Enemy : public mono::PhysicsEntityBase
    {
    public:

        Enemy(EnemySetup& setup);

        virtual void Draw(mono::IRenderer& renderer) const;
        virtual void Update(unsigned int delta);

        std::unique_ptr<IEnemyController> m_controller;
        mono::ISpritePtr m_sprite;
    };
}
