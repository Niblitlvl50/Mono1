
#pragma once

#include "ISprite.h"
#include "Math/Quad.h"
#include "Rendering/Color.h"
#include "Rendering/Texture/ITextureFactory.h"

namespace mono
{
    class Sprite : public ISprite
    {
    public:

        Sprite();
        Sprite(const SpriteData* sprite_data, mono::ITexturePtr texture);
        void Init(const SpriteData* sprite_data, mono::ITexturePtr texture);
        void SetUserId(uint32_t id);

        uint32_t GetSpriteHash() const override;
        ITexture* GetTexture() const override;
        SpriteFrame GetCurrentFrame() const override;
        int GetCurrentFrameIndex() const override;
        const Color::RGBA& GetShade() const override;
        void SetShade(const mono::Color::RGBA& color) override;
        math::Vector GetShadowOffset() const override;
        void SetShadowOffset(const math::Vector& shadow_offset) override;
        uint32_t GetProperties() const override;
        void SetProperties(uint32_t properties) override;
        void SetProperty(mono::SpriteProperty property) override;
        bool HasProperty(mono::SpriteProperty property) const override;
        void ClearProperty(mono::SpriteProperty property) override;
        void SetAnimation(int id) override;
        void SetAnimation(const char* name) override;
        void SetAnimation(int id, const SpriteAnimationCallback& callback) override;
        void SetAnimation(const char* name, const SpriteAnimationCallback& callback) override;
        void SetNotifyCallback(const SpriteAnimationNotifyCallback& callback) override;
        void SetAnimationPlaybackSpeed(float speed_scale) override;
        void SetAnimationPlayback(PlaybackMode mode) override;
        int GetActiveAnimation() const override;
        float GetAnimationLengthSeconds(int animation_id) const override;
        int GetAnimationIdFromName(const char* animation_name) const override;
        void SetActiveAnimationFrame(int frame) override;
        int GetActiveAnimationFrame() const override;
        bool IsActiveAnimationDone() const override;
        void RestartAnimation() override;
        void Update(const UpdateContext& update_context) override;
        const SpriteData* GetSpriteData() const override;

        int GetDefinedAnimations() const;
        SpriteFrame GetFrame(int frame_index) const;

        void SetShadowSize(float size);
        float GetShadowSize() const override;

        void FlashSprite();
        bool ShouldFlashSprite() const override;

        void SetOutlineSprite(bool outline) override;
        bool ShouldOutlineSprite() const override;

    private:

        const SpriteData* m_sprite_data;
        mono::ITexturePtr m_texture;
        uint32_t m_sprite_user_id;

        int m_active_animation;
        bool m_active_animation_done;
        int m_active_frame;
        int m_active_frame_time;
        float m_playback_speed;
        PlaybackMode m_playback_mode;

        uint32_t m_properties;
        Color::RGBA m_color;
        math::Vector m_shadow_offset;
        float m_shadow_size;
        float m_flash_timer;
        bool m_outline;
        SpriteAnimationCallback m_callback;
        SpriteAnimationNotifyCallback m_notify_callback;
    };
}
