
#pragma once

#include "IUpdatable.h"
#include "Rendering/RenderPtrFwd.h"
#include "Math/Quad.h"
#include "Math/Vector.h"

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

    struct SpriteFrame
    {
        math::Quad texture_coordinates;
        math::Vector center_offset;
        math::Vector size;
    };

    class ISprite : public IUpdatable
    {
    public:
        
        //! Gets the sprites texture
        virtual mono::ITexturePtr GetTexture() const = 0;

        //! Gets the quad representing the texture coordinates.
        virtual mono::SpriteFrame GetCurrentFrame() const = 0;

        //! Gets the color shade of the sprite
        virtual const mono::Color::RGBA& GetShade() const = 0;

        //! Sets the color shade of the sprite
        virtual void SetShade(const mono::Color::RGBA& color) = 0;

        virtual void SetHorizontalDirection(mono::HorizontalDirection direction) = 0;
        virtual void SetVerticalDirection(mono::VerticalDirection direction) = 0;

        //! Tell the sprite to run a specific animation.
        virtual void SetAnimation(int id) = 0;
        virtual void SetAnimation(const char* name) = 0;

        //! Tell the sprite to run a specific animation, and get a callback when finished
        virtual void SetAnimation(int id, const std::function<void ()>& func) = 0;
        virtual void SetAnimation(const char* name, const std::function<void ()>& func) = 0;

        //! Returns the current active animation
        virtual int GetActiveAnimation() const = 0;
    };
}
