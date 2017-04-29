
#pragma once

#include "Entity/EntityBase.h"
#include "Particle/ParticleFwd.h"

namespace game
{
    class TrailEffect : public mono::EntityBase
    {
    public:

        TrailEffect(const math::Vector& position);
        virtual ~TrailEffect();

        virtual void Draw(mono::IRenderer& renderer) const;
        virtual void Update(unsigned int delta);
        virtual math::Quad BoundingBox() const;

        const math::Vector& m_position;
        std::unique_ptr<mono::ParticlePool> m_pool;
        std::unique_ptr<mono::ParticleEmitter> m_emitter;
    };
}
