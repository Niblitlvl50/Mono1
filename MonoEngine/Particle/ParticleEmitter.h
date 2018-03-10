
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
            float emit_rate = 0.2f;
            ParticleGenerator generator = nullptr;
            const void* generator_context = nullptr;
        };

        ParticleEmitter(const Configuration& config, ParticlePool& pool);
        void SetPosition(const math::Vector& position);

        void doUpdate(unsigned int delta) override;

    private:
    
        const Configuration m_config;
        math::Vector m_position;
        ParticlePool& m_pool;
    };
}
