
#pragma once

#include "Particle/ParticleFwd.h"

#include "IUpdatable.h"
#include "Rendering/RenderPtrFwd.h"
#include "Rendering/IDrawable.h"
#include "Math/Vector.h"

namespace mono
{
    class IRenderBuffer;

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
        ~ParticleEmitter();

        void SetPosition(const math::Vector& position);

        virtual void doUpdate(unsigned int delta);
        virtual void doDraw(mono::IRenderer& renderer) const;
        virtual math::Quad BoundingBox() const;

        const Configuration m_config;
        math::Vector m_position;
        ParticlePool& m_pool;

        std::unique_ptr<IRenderBuffer> m_positionBuffer;
        std::unique_ptr<IRenderBuffer> m_colorBuffer;
    };
}
