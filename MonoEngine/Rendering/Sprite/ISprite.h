
#pragma once

#include "IUpdatable.h"
#include "Rendering/RenderPtrFwd.h"
#include "Math/MathFwd.h"

#include <functional>

namespace mono
{
    class ISprite : public IUpdatable
    {
    public:
        
        //! Gets the sprites texture
        //! @return ITexturePtr A shared pointer to the texture.
        virtual ITexturePtr GetTexture() const = 0;

        //! Gets the quad representing the texture coordinates,
        //! the sprite can return zeroQuad for texture coordinates
        //! if the animation is finished.
        //! @return Math::Quad A reference to the texture coords.
        virtual const math::Quad& GetTextureCoords() const = 0;

        virtual const math::Quad& GetFullTexureCoords() const = 0;

        //! Gets the color shade of the sprite
        //! @return Color A reference to the color shading.
        virtual const Color::RGBA& GetShade() const = 0;

        //! Sets the color shade of the sprite
        //! @param[in] color The color shading
        virtual void SetShade(const mono::Color::RGBA& color) = 0;

        //! Tell the sprite to run a specific animation.
        virtual void SetAnimation(int id) = 0;
        virtual void SetAnimation(const char* name) = 0;

        //! Tell the sprite to run a specific animation, and get a callback when finished
        //! @param[in] id The animation to run.
        //! @param[in] func A callback function.
        virtual void SetAnimation(int id, const std::function<void ()>& func) = 0;

        //! Restarts the current set animation
        virtual void RestartAnimation() = 0;

        //! Returns the current active animation
        virtual int GetActiveAnimation() const = 0;
    };
}
