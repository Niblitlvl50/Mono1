
#pragma once

#include "Rendering/IDrawable.h"
#include "Rendering/Sprite/Sprite.h"

#include "Entity/EntityBase.h"


namespace animator
{
    class SpriteTexture : public mono::EntityBase
    {
    public:

        SpriteTexture(const mono::ITexturePtr& texture, const math::Quad& full_texture);

        virtual void Draw(mono::IRenderer& renderer) const;
        virtual void Update(unsigned int delta);

        void Enable(bool enable);
        bool Enabled() const;

        mono::Sprite m_texture;

        bool m_enabled = false;
        float m_width;
        float m_height;
    };
}
