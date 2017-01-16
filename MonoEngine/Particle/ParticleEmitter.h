
#pragma once

#include "MonoPtrFwd.h"
#include "Particle/ParticleFwd.h"

#include "IUpdatable.h"
#include "IDrawable.h"
#include "Math/Vector.h"

namespace mono
{
    class ParticleEmitter : public mono::IUpdatable, public mono::IDrawable
    {
    public:

        struct Configuration
        {
            math::Vector position;
            ParticleGenerator generator = nullptr;
            ParticleUpdater updater = nullptr;
            mono::ITexturePtr texture;
            float emit_rate = 0.2f;
            float point_size = 1.0f;
        };

        ParticleEmitter(const Configuration& config, ParticlePool& pool);

        virtual void doUpdate(unsigned int delta);
        virtual void doDraw(mono::IRenderer& renderer) const;
        virtual math::Quad BoundingBox() const;

        const Configuration m_config;
        ParticlePool& m_pool;
    };
}
