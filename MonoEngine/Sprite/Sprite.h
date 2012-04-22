//
//  AnimatedSprite.h
//  Mono1
//
//  Created by Niblit on 2012-04-09.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//


//!
//! An image with 2 rows and 5 columns and their indexes. 
//!  _ _ _ _ _ 
//! |0|1|2|3|4|
//!  - - - - - 
//! |5|6|7|8|9|
//!  - - - - -
//!
//!


#pragma once

#include "MonoFwd.h"
#include "IUpdatable.h"
#include "Vector2f.h"
#include "AnimationSequence.h"

#include <map>

namespace mono
{    
    struct SpriteTextureCoord
    {
        Math::Vector2f bl;
        Math::Vector2f tl;
        Math::Vector2f tr;
        Math::Vector2f br;
    };

    class Sprite : public IUpdatable
    {
    public:

        Sprite(const std::string& file);
        Sprite(const std::string& file, int rows, int columns);

        void Draw() const;
        void DefineAnimation(int id, unsigned int start, unsigned int end, const FrameDurations& durations);
        void SetAnimation(int id);
        
        virtual void doUpdate(unsigned int delta);

    private:
        
        void Init(const std::string& file, int rows, int columns);
        
        int mActiveAnimationId;        
        ITexturePtr mTexture;
        
        std::vector<SpriteTextureCoord> mTextureCoordinates;
        std::map<int, AnimationSequence> mDefinedAnimations;
    };
}

