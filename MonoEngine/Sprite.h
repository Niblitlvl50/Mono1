//
//  Sprite.h
//  Mono1
//
//  Created by Niblit on 2012-04-06.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//


#ifndef MONO_SPRITE_H
#define MONO_SPRITE_H

#include "MonoFwd.h"

namespace mono
{
    class Sprite
    {
    public:
        
        Sprite(const std::string& file, float xscale, float yscale);
        void DrawAt(float x, float y) const;
        
    private:
        
        float mXScale;
        float mYScale;
        ITexturePtr mTexture;
    };
}

#endif

