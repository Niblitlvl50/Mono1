
#pragma once

#include "ISprite.h"
#include "AnimationSequence.h"
#include "Math/Quad.h"
#include "Rendering/Color.h"

#include <functional>

namespace mono
{    
    class Sprite : public ISprite
    {
    public:

        Sprite();

        //! Constructs a sprite object
        //! @param[in] texture The texture to use for this sprite.
        //! @param[in] coordinates The texture coordinates
        Sprite(const mono::ITexturePtr& texture, const std::vector<SpriteFrame>& sprite_frames);
        void Init(const mono::ITexturePtr& texture, const std::vector<SpriteFrame>& sprite_frames);

        ITexturePtr GetTexture() const override;
        SpriteFrame GetCurrentFrame() const override;
        const Color::RGBA& GetShade() const override;
        void SetShade(const mono::Color::RGBA& color) override;
        void SetHorizontalDirection(HorizontalDirection direction) override;
        void SetVerticalDirection(VerticalDirection direction) override;
        void SetAnimation(int id) override;
        void SetAnimation(const char* name) override;
        void SetAnimation(int id, const std::function<void ()>& func) override;
        void SetAnimation(const char* name, const std::function<void ()>& func) override;
        int GetActiveAnimation() const override;
        void doUpdate(uint32_t delta_ms) override;

        //! Restarts the current set animation
        void RestartAnimation();

        //! Define an animation sequence by giving pairs of frame and duration.
        //! @param[in] frames A collection of frame and duration pairs. 
        //! @param[in] loop If the animation should loop or not.
        //! return The create animation id
        int DefineAnimation(const std::string& name, const std::vector<int>& frames, bool loop);

        //! Returns the number of defined animations for this sprite
        int GetDefinedAnimations() const;

        int GetUniqueFrames() const;
        SpriteFrame GetFrame(int frame_index) const;
        void SetFrameOffset(int frame_index, const math::Vector& offset);

        const AnimationSequence& GetSequence(int id) const;
              AnimationSequence& GetSequence(int id);

        const std::vector<AnimationSequence>& GetAnimations() const;
              std::vector<AnimationSequence>& GetAnimations();

    private:

        int FindAnimationByName(const char* name) const;

        // Instance data
        int m_active_animation;
        bool m_flip_horizontal;
        bool m_flip_vertical;
        Color::RGBA m_color;
        std::function<void ()> m_callback;
        std::vector<AnimationSequence> m_animations; // Animations could be considered shared as well

        // Shared data
        ITexturePtr m_texture;
        std::vector<SpriteFrame> m_sprite_frames;
    };
}

