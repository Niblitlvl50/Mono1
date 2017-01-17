
#pragma once

#include "EntityBase.h"
#include "Particle/ParticleFwd.h"
#include "Particle/ParticlePool.h"

namespace game
{
    class SmokeEffect : public mono::EntityBase
    {
    public:

        SmokeEffect(const math::Vector& position);

        virtual void Draw(mono::IRenderer& renderer) const;
        virtual void Update(unsigned int delta);

        mono::ParticlePool m_pool;
        std::shared_ptr<mono::ParticleEmitter> m_emitter;
    };
}
