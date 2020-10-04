
#pragma once

#include "ISprite.h"
#include "Math/Quad.h"
#include "Rendering/Color.h"

namespace mono
{
    class Sprite : public ISprite
    {
    public:

        Sprite();
        Sprite(const SpriteData* sprite_data, mono::ITexturePtr texture);
        void Init(const SpriteData* sprite_data, mono::ITexturePtr texture);

        uint32_t GetSpriteHash() const override;
        ITexturePtr GetTexture() const override;
        SpriteFrame GetCurrentFrame() const override;
        const Color::RGBA& GetShade() const override;
        void SetShade(const mono::Color::RGBA& color) override;
        void SetHorizontalDirection(HorizontalDirection direction) override;
        mono::HorizontalDirection GetHorizontalDirection() const override;
        void SetVerticalDirection(VerticalDirection direction) override;
        mono::VerticalDirection GetVerticalDirection() const override;
        void SetAnimation(int id) override;
        void SetAnimation(const char* name) override;
        void SetAnimation(int id, const SpriteAnimationCallback& callback) override;
        void SetAnimation(const char* name, const SpriteAnimationCallback& callback) override;
        int GetActiveAnimation() const override;
        void RestartAnimation() override;
        void Update(const UpdateContext& update_context) override;

        int GetDefinedAnimations() const;
        SpriteFrame GetFrame(int frame_index) const;

        /*
        void SetFrameOffset(int frame_index, const math::Vector& offset);
        */

    private:

        const SpriteData* m_sprite_data;
        mono::ITexturePtr m_texture;

        int m_active_animation;
        bool m_active_animation_done;
        int m_active_frame;
        int m_active_frame_time;

        bool m_flip_horizontal;
        bool m_flip_vertical;
        Color::RGBA m_color;
        SpriteAnimationCallback m_callback;
    };
}
