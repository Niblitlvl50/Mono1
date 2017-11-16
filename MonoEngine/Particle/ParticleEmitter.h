
#pragma once

#include "Particle/ParticleFwd.h"

#include "IUpdatable.h"
#include "Math/Vector.h"

namespace mono
{
    class ParticleEmitter : public mono::IUpdatable
    {
    public:

        struct Configuration
        {
            math::Vector position;
            ParticleGenerator generator = nullptr;
            ParticleUpdater updater = nullptr;
            float emit_rate = 0.2f;
        };

        ParticleEmitter(const Configuration& config, ParticlePool& pool);
        void SetPosition(const math::Vector& position);

        void doUpdate(unsigned int delta) override;

        const Configuration m_config;
        math::Vector m_position;
        ParticlePool& m_pool;
    };
}
