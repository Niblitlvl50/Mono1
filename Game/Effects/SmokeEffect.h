
#pragma once

#include "Entity/EntityBase.h"
#include "Particle/ParticleFwd.h"

namespace game
{
    class SmokeEffect : public mono::EntityBase
    {
    public:

        SmokeEffect(const math::Vector& position);
        virtual ~SmokeEffect();

        virtual void Draw(mono::IRenderer& renderer) const;
        virtual void Update(unsigned int delta);

        std::unique_ptr<mono::ParticlePool> m_pool;
        std::unique_ptr<mono::ParticleEmitter> m_emitter;
    };
}
