
#pragma once

#include "Rendering/IDrawable.h"
#include "Rendering/RenderFwd.h"
#include <unordered_map>
#include <cstdint>

namespace mono
{
    class ParticleSystem;

    class ParticleSystemDrawer : public mono::IDrawable
    {
    public:

        ParticleSystemDrawer(const mono::ParticleSystem* particle_system);
        ~ParticleSystemDrawer();

        void doDraw(mono::IRenderer& renderer) const override;
        math::Quad BoundingBox() const override;

    private:

        struct InternalRenderData
        {
            std::unique_ptr<IRenderBuffer> position_buffer;
            std::unique_ptr<IRenderBuffer> rotation_buffer;
            std::unique_ptr<IRenderBuffer> color_buffer;
            std::unique_ptr<IRenderBuffer> point_size_buffer;
        };

        const mono::ParticleSystem* m_particle_system;
        mutable std::unordered_map<uint32_t, InternalRenderData> m_render_data;
    };
}
