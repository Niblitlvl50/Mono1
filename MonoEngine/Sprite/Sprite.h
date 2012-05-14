//
//  AnimatedSprite.h
//  Mono1
//
//  Created by Niblit on 2012-04-09.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//


//!
//! An image with 2 rows and 5 columns and their indices. 
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
#include "AnimationSequence.h"
#include "Quad.h"

#include <map>


namespace mono
{    
    class Sprite : public IUpdatable
    {
    public:

        Sprite(const std::string& file);

        void Draw() const;
        void DefineAnimation(int id, const std::vector<int>& values);
        void SetAnimation(int id);
        
        virtual void doUpdate(unsigned int delta);

    private:
                
        int mActiveAnimationId;        
        ITexturePtr mTexture;
        
        std::vector<Math::Quad> mTextureCoordinates;
        std::map<int, AnimationSequence> mDefinedAnimations;
    };
}

