
#pragma once

#include "EntityBase.h"
#include "Particle/ParticleFwd.h"

namespace game
{
    class TrailEffect : public mono::EntityBase
    {
    public:

        TrailEffect();
        virtual ~TrailEffect();

        virtual void Draw(mono::IRenderer& renderer) const;
        virtual void Update(unsigned int delta);
        virtual math::Quad BoundingBox() const;

        std::unique_ptr<mono::ParticlePool> m_pool;
        std::unique_ptr<mono::ParticleEmitter> m_emitter;
    };
}
