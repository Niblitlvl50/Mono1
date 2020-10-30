
#pragma once

#include "IUpdatable.h"
#include "Rendering/RenderFwd.h"
#include "Rendering/Texture/ITextureFactory.h"
#include "Math/Quad.h"
#include "Math/Vector.h"

#include "SpriteData.h"

#include <functional>

namespace mono
{
    enum class HorizontalDirection
    {
        LEFT,
        RIGHT
    };

    enum class VerticalDirection
    {
        UP,
        DOWN
    };

    enum class PlaybackMode
    {
        PLAYING,
        PAUSED
    };

    using SpriteAnimationCallback = std::function<void()>;

    class ISprite : public IUpdatable
    {
    public:
        
        // Hash of the filename used to initialize this sprite
        virtual uint32_t GetSpriteHash() const = 0;

        //! Gets the sprites texture
        virtual mono::ITexturePtr GetTexture() const = 0;

        //! Gets the quad representing the texture coordinates.
        virtual mono::SpriteFrame GetCurrentFrame() const = 0;

        //! Gets the color shade of the sprite
        virtual const mono::Color::RGBA& GetShade() const = 0;

        //! Sets the color shade of the sprite
        virtual void SetShade(const mono::Color::RGBA& color) = 0;

        virtual uint32_t GetProperties() const = 0;
        virtual void SetProperties(uint32_t properties) = 0;

        virtual void SetHorizontalDirection(mono::HorizontalDirection direction) = 0;
        virtual mono::HorizontalDirection GetHorizontalDirection() const = 0;

        virtual void SetVerticalDirection(mono::VerticalDirection direction) = 0;
        virtual mono::VerticalDirection GetVerticalDirection() const = 0;

        //! Tell the sprite to run a specific animation.
        virtual void SetAnimation(int id) = 0;
        virtual void SetAnimation(const char* name) = 0;

        //! Tell the sprite to run a specific animation, and get a callback when finished
        virtual void SetAnimation(int id, const SpriteAnimationCallback& callback) = 0;
        virtual void SetAnimation(const char* name, const SpriteAnimationCallback& callback) = 0;

        virtual void SetAnimationPlayback(PlaybackMode mode) = 0;

        //! Returns the current active animation
        virtual int GetActiveAnimation() const = 0;

        virtual void SetActiveAnimationFrame(int frame) = 0;
        virtual int GetActiveAnimationFrame() const = 0;
        
        virtual void RestartAnimation() = 0;
    };
}
