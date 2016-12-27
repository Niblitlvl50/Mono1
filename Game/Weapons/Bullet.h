
#pragma once

#include "MonoPtrFwd.h"
#include "PhysicsEntityBase.h"
#include "Physics/IBody.h"

#include "WeaponConfiguration.h"

namespace game
{
    class Bullet : public mono::PhysicsEntityBase, public mono::ICollisionHandler
    {
    public:

        Bullet(const BulletConfiguration& config);

        virtual void Draw(mono::IRenderer& renderer) const;
        virtual void Update(unsigned int delta);

        virtual void OnCollideWith(const mono::IBodyPtr& body);
        virtual void OnPostStep();

    private:
        mono::ISpritePtr m_sprite;
        mono::ISoundPtr m_sound;

        BulletImpactCallback m_collisionCallback;
        int m_lifeSpan;
    };
}
