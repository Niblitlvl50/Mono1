
#pragma once

#include "Rendering/IDrawable.h"
#include "Rendering/RenderPtrFwd.h"
#include "Particle/ParticleFwd.h"

namespace mono
{
    class IRenderBuffer;

    class ParticleDrawer : public mono::IDrawable
    {
    public:

        ParticleDrawer(const mono::ITexturePtr& texture, ParticlePool& pool);
        ~ParticleDrawer();

        void doDraw(mono::IRenderer& renderer) const override;
        math::Quad BoundingBox() const override;

        const mono::ITexturePtr m_texture;
        ParticlePool& m_pool;

        std::unique_ptr<IRenderBuffer> m_positionBuffer;
        std::unique_ptr<IRenderBuffer> m_colorBuffer;
        std::unique_ptr<IRenderBuffer> m_point_size_buffer;
    };
}
