//
//  AnimatedSprite.h
//  Mono1
//
//  Created by Niblit on 2012-04-09.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

//! This is an example of a sprite file that you pass to this sprite class

//! 
//! texture = "sheet.png"
//! rows = 3
//! columns = 4
//!
//! x = 1
//! y = 1
//! u = 128
//! v = 98
//!
//! animations = { }
//! animations[0] = { 0, -1 }
//! animations[1] = { 1, 100, 2, 100, 3, 100 }
//! animations[2] = { 4, 100, 5, 100, 6, 100, 7, 100 }
//! animations[3] = { 8, 100, 9, 100, 10, 100, 11, 100 }
//!

//!
//! An image with 2 rows and 5 columns and their indices. 
//!  _ _ _ _ _ 
//! |0|1|2|3|4|
//!  - - - - - 
//! |5|6|7|8|9|
//!  - - - - -
//!


#pragma once

#include "MonoPtrFwd.h"
#include "IUpdatable.h"
#include "AnimationSequence.h"
#include "Quad.h"
#include "Color.h"

#include <string>
#include <map>
#include <functional>

namespace mono
{    
    class Sprite : public IUpdatable
    {
    public:

        //! Constructs a sprite object.
        //! @param[in] file The .sprite file to use.
        Sprite(const std::string& file);
        
        //! Gets the sprites texture
        //! @return ITexturePtr A shared pointer to the texture.
        ITexturePtr GetTexture() const;
        
        //! Gets the quad representing the texture coordinates
        //! @return Math::Quad A reference to the texture coords.
        const math::Quad& GetTextureCoords() const;
        
        //! Gets the color shade of the sprite
        //! @return Color A reference to the color shading.
        const Color& GetShade() const;
        
        //! Sets the color shade of the sprite
        //! @param[in] color The color shading
        void SetShade(const mono::Color& color);
        
        //! Define an animation sequence by giving pairs of frame and duration.
        //! @param[in] id Id of the animation.
        //! @param[in] frames A collection of frame and duration pairs. 
        //! @param[in] loop If the animation should loop or not.
        void DefineAnimation(int id, const std::vector<int>& frames, bool loop);

        //! Define an animation sequence by giving pairs of frame and duration, also with attributes.
        //! @param[in] id Id of the animation.
        //! @param[in] frames A collection of frame and duration pairs.
        //! @param[in] attributes A collection of attributes for this animation.
        void DefineAnimation(int id, const std::vector<int>& frames, const std::vector<std::string>& attributes);
        
        //! Tell the sprite to run a specific animation.
        //! @param[in] id The animation to run.
        void SetAnimation(int id);
        
        //! Tell the sprite to run a specific animation, and get a callback when finished
        //! @param[in] id The animation to run.
        //! @param[in] func A callback function.
        void SetAnimation(int id, std::function<void ()> func);
        
        virtual void doUpdate(unsigned int delta);

    private:
                
        int mActiveAnimationId;
        std::function<void ()> mCallbackFunction;
        ITexturePtr mTexture;
        
        std::vector<math::Quad> mTextureCoordinates;
        std::map<int, AnimationSequence> mDefinedAnimations;

        Color mColor;
    };
}

