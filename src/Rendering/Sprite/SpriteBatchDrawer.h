
#pragma once

#include "MonoFwd.h"
#include "Rendering/IDrawable.h"

namespace mono
{
    class SpriteBatchDrawer : public mono::IDrawable
    {
    public:

        SpriteBatchDrawer(const mono::TransformSystem* transform_system, mono::SpriteSystem* sprite_system);
        void Draw(mono::IRenderer& renderer) const override;
        math::Quad BoundingBox() const override;

    private:
        const mono::TransformSystem* m_transform_system;
        mono::SpriteSystem* m_sprite_system;
    };
}
