//
//  ISprite.h
//  MonoOSX
//
//  Created by Niklas Damberg on 19/07/15.
//
//

#pragma once

#include "IUpdatable.h"
#include "MonoPtrFwd.h"
#include "Math/MathFwd.h"

#include <functional>
#include <vector>

namespace mono
{
    class AnimationSequence;

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

        //! Gets the color shade of the sprite
        //! @return Color A reference to the color shading.
        virtual const Color::RGBA& GetShade() const = 0;

        //! Sets the color shade of the sprite
        //! @param[in] color The color shading
        virtual void SetShade(const mono::Color::RGBA& color) = 0;

        //! Tell the sprite to run a specific animation.
        //! @param[in] id The animation to run.
        virtual void SetAnimation(int id) = 0;

        //! Tell the sprite to run a specific animation, and get a callback when finished
        //! @param[in] id The animation to run.
        //! @param[in] func A callback function.
        virtual void SetAnimation(int id, const std::function<void ()>& func) = 0;

        //! Restarts the current set animation
        virtual void RestartAnimation() = 0;

        //! Returns the number of defined animations for this sprite
        //! @return int
        virtual int GetDefinedAnimations() const = 0;

        //! Returns the current active animation
        //! @return int
        virtual int GetActiveAnimation() const = 0;

        //! Get an animation sequence from an animation id
        virtual const AnimationSequence& GetSequence(int id) const = 0;
        virtual       AnimationSequence& GetSequence(int id)       = 0;

        virtual const std::vector<AnimationSequence>& GetAnimations() const = 0;
    };
}
