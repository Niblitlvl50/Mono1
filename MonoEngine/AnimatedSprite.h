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

    typedef std::vector<unsigned int> FrameDurations;
    
    struct AnimationSequence
    {
        AnimationSequence()
            : mStart(0),
              mEnd(0),
              mFrame(0),
              mElapsedTime(0)
        { }
        
        AnimationSequence(unsigned int start, unsigned int end, const FrameDurations& durations)
            : mStart(start),
              mEnd(end),
              mFrame(start),
              mElapsedTime(0),
              mDurations(durations)
        { }
        
        unsigned int mStart;
        unsigned int mEnd;
        unsigned int mFrame;
        unsigned int mElapsedTime;
        FrameDurations mDurations;
    };

    class AnimatedSprite : public IUpdatable
    {
    public:

        AnimatedSprite(const std::string& file, int rows, int columns, float xscale, float yscale);

        void DrawAt(float x, float y) const;
        void DefineAnimation(int id, unsigned int start, unsigned int end, const FrameDurations& durations);
        void SetAnimation(int id);
        
        virtual void Update(unsigned int delta);

    private:
        
        float mXScale;
        float mYScale;
        int mActiveAnimationId;
        
        ITexturePtr mTexture;
        
        std::vector<SpriteTextureCoord> mTextureCoordinates;
        std::map<int, AnimationSequence> mDefinedAnimations;
    };
}

