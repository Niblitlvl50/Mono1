
#pragma once

#include "Rendering/IDrawable.h"
#include "Rendering/RenderPtrFwd.h"
#include "Rendering/BlendMode.h"
#include "Particle/ParticleFwd.h"

namespace mono
{
    class IRenderBuffer;

    class ParticleDrawer : public mono::IDrawable
    {
    public:

        ParticleDrawer(const mono::ITexturePtr& texture, mono::BlendMode blend_mode, ParticlePool& pool);
        ~ParticleDrawer();

        void doDraw(mono::IRenderer& renderer) const override;
        math::Quad BoundingBox() const override;

        const mono::ITexturePtr m_texture;
        const mono::BlendMode m_blend_mode;
        ParticlePool& m_pool;

        std::unique_ptr<IRenderBuffer> m_position_buffer;
        std::unique_ptr<IRenderBuffer> m_rotation_buffer;
        std::unique_ptr<IRenderBuffer> m_color_buffer;
        std::unique_ptr<IRenderBuffer> m_point_size_buffer;
    };
}
