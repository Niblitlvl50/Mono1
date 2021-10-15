
#pragma once

#include "MonoFwd.h"
#include "Rendering/IDrawable.h"
#include "Rendering/RenderFwd.h"

#include <unordered_map>
#include <vector>
#include <cstdint>
#include <memory>

namespace mono
{
    class ParticleSystemDrawer : public mono::IDrawable
    {
    public:

        ParticleSystemDrawer(const mono::ParticleSystem* particle_system, const mono::TransformSystem* transform_system);
        ~ParticleSystemDrawer();

        void Draw(mono::IRenderer& renderer) const override;
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
        const mono::TransformSystem* m_transform_system;
        mutable std::unordered_map<uint32_t, InternalRenderData> m_render_data;
        mutable std::vector<uint32_t> m_last_active_particles;
    };
}
