
#pragma once

#include "IUpdatable.h"
#include "Rendering/RenderFwd.h"
#include "SpriteData.h"

#include <functional>

namespace mono
{
    enum class PlaybackMode
    {
        PAUSED,
        PLAYING
    };

    using SpriteAnimationCallback = std::function<void()>;

    class ISprite : public IUpdatable
    {
    public:
        
        // Hash of the filename used to initialize this sprite
        virtual uint32_t GetSpriteHash() const = 0;

        //! Gets the sprites texture
        virtual mono::ITexture* GetTexture() const = 0;

        //! Gets the quad representing the texture coordinates.
        virtual mono::SpriteFrame GetCurrentFrame() const = 0;

        virtual int GetCurrentFrameIndex() const = 0;

        virtual const mono::Color::RGBA& GetShade() const = 0;
        virtual void SetShade(const mono::Color::RGBA& color) = 0;

        virtual math::Vector GetShadowOffset() const = 0;
        virtual float GetShadowSize() const = 0;

        virtual uint32_t GetProperties() const = 0;
        virtual void SetProperties(uint32_t properties) = 0;

        //! Tell the sprite to run a specific animation.
        virtual void SetAnimation(int animation_id) = 0;
        virtual void SetAnimation(const char* animation_name) = 0;

        //! Tell the sprite to run a specific animation, and get a callback when finished
        virtual void SetAnimation(int animation_id, const SpriteAnimationCallback& callback) = 0;
        virtual void SetAnimation(const char* animation_name, const SpriteAnimationCallback& callback) = 0;

        virtual void SetAnimationPlayback(PlaybackMode mode) = 0;
        virtual void SetAnimationPlaybackSpeed(float speed_scale) = 0;

        //! Returns the current active animation
        virtual int GetActiveAnimation() const = 0;

        //! Returns length in ms
        virtual uint32_t GetAnimationLength(int animation_id) const = 0;
        virtual uint32_t GetAnimationLength(const char* animation_name) const = 0;

        // Will return -1 on failure. 
        virtual int GetAnimationIdFromName(const char* animation_name) const = 0;

        virtual void SetActiveAnimationFrame(int frame) = 0;
        virtual int GetActiveAnimationFrame() const = 0;
        virtual bool IsActiveAnimationDone() const = 0;

        virtual bool ShouldFlashSprite() const = 0;
        
        virtual void RestartAnimation() = 0;

        virtual const SpriteData* GetSpriteData() const = 0;
    };
}
