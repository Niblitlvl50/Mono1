//
//  AnimatedSprite.h
//  Mono1
//
//  Created by Niblit on 2012-04-09.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//



#pragma once

#include "ISprite.h"
#include "AnimationSequence.h"
#include "Math/Quad.h"
#include "Color.h"

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
        Sprite(const mono::ITexturePtr& texture, const std::vector<math::Quad>& coordinates);

        void Init(const mono::ITexturePtr& texture, const std::vector<math::Quad>& coordinates);

        //! Gets the sprites texture
        //! @return ITexturePtr A shared pointer to the texture.
        virtual ITexturePtr GetTexture() const;
        
        //! Gets the quad representing the texture coordinates
        //! @return Math::Quad A reference to the texture coords.
        virtual const math::Quad& GetTextureCoords() const;
        
        //! Gets the color shade of the sprite
        //! @return Color A reference to the color shading.
        virtual const Color::RGBA& GetShade() const;
        
        //! Sets the color shade of the sprite
        //! @param[in] color The color shading
        virtual void SetShade(const mono::Color::RGBA& color);
        
        //! Tell the sprite to run a specific animation.
        //! @param[in] id The animation to run.
        virtual void SetAnimation(int id);
        
        //! Tell the sprite to run a specific animation, and get a callback when finished
        //! @param[in] id The animation to run.
        //! @param[in] func A callback function.
        virtual void SetAnimation(int id, const std::function<void ()>& func);

        //! Restarts the current set animation
        virtual void RestartAnimation();

        //! Returns the current active animation
        virtual int GetActiveAnimation() const;

        //! Define an animation sequence by giving pairs of frame and duration.
        //! @param[in] frames A collection of frame and duration pairs. 
        //! @param[in] loop If the animation should loop or not.
        //! return The create animation id
        int DefineAnimation(const std::string& name, const std::vector<int>& frames, bool loop);

        //! Returns the number of defined animations for this sprite
        int GetDefinedAnimations() const;

        int GetUniqueFrames() const;

        const AnimationSequence& GetSequence(int id) const;
              AnimationSequence& GetSequence(int id);

        const std::vector<AnimationSequence>& GetAnimations() const;
              std::vector<AnimationSequence>& GetAnimations();

        virtual void doUpdate(unsigned int delta);

    private:

        int m_activeAnimation;
        std::function<void ()> m_callback;
        ITexturePtr m_texture;

        std::vector<math::Quad> m_textureCoordinates;
        std::vector<AnimationSequence> m_animations;

        Color::RGBA m_color;
    };
}

