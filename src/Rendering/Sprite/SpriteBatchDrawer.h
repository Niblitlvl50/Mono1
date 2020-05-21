
#pragma once

#include "Rendering/IDrawable.h"

namespace mono
{
    class TransformSystem;
    class SpriteSystem;

    class SpriteBatchDrawer : public mono::IDrawable
    {
    public:

        SpriteBatchDrawer(class SystemContext* system_context);

        void Draw(mono::IRenderer& renderer) const override;
        math::Quad BoundingBox() const override;

    private:
        mono::TransformSystem* m_transform_system;
        mono::SpriteSystem* m_sprite_system;
    };
}
