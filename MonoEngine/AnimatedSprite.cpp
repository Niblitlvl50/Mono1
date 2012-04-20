//
//  AnimatedSprite.cpp
//  Mono1
//
//  Created by Niblit on 2012-04-09.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "AnimatedSprite.h"
#include "ITexture.h"
#include "TextureFactory.h"
#include "SysOpenGL.h"

using namespace mono;

namespace
{
    static const float vertices[] = { -1.0f, -1.0f,
                                      -1.0f,  1.0f,
                                       1.0f,  1.0f,
                                       1.0f, -1.0f };
        
    static const unsigned short indices[] = { 0, 1, 2, 0, 2, 3 };
    
    void GenerateTextureCoordinates(int rows, int columns, std::vector<SpriteTextureCoord>& coordinates)
    {
        const float ystep = 1.0f / rows;
        const float xstep = 1.0f / columns;
        
        for(int yindex = rows; yindex > 0; --yindex)
        {
            for(int xindex = 0; xindex < columns; ++xindex)
            {
                const float x = xstep * xindex;
                const float y = ystep * (yindex -1);
                const float u = xstep * (xindex +1);
                const float v = ystep * yindex;
                
                SpriteTextureCoord coord;
                coord.bl = Math::Vector2f(x, y);
                coord.tl = Math::Vector2f(x, v);
                coord.tr = Math::Vector2f(u, v);
                coord.br = Math::Vector2f(u, y);
                
                coordinates.push_back(coord);
            }
        }
    }

}


AnimatedSprite::AnimatedSprite(const std::string& file, int rows, int columns, float xscale, float yscale)
    : mXScale(xscale),
      mYScale(yscale),
      mActiveAnimationId(0)
{
    mTexture = mono::CreateTexture(file);
    GenerateTextureCoordinates(rows, columns, mTextureCoordinates);
}

void AnimatedSprite::DrawAt(float x, float y) const
{
    const AnimationSequence& anim = mDefinedAnimations.find(mActiveAnimationId)->second;
    const SpriteTextureCoord& texcoords = mTextureCoordinates.at(anim.mFrame);
    
    mTexture->Use();
    
    glPushMatrix();
    
    glTranslatef(x, y, 0.0f);
    glScalef(mXScale, mYScale, 0.0f);
    glColor3f(1.0f, 1.0f, 1.0f);
    
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    
    glTexCoordPointer(2, GL_FLOAT, 0, &texcoords);
    glVertexPointer(2, GL_FLOAT, 0, vertices);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, indices);

    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
    
    glPopMatrix();
}

void AnimatedSprite::Update(unsigned int delta)
{
    AnimationSequence& anim = mDefinedAnimations[mActiveAnimationId];
    anim.mElapsedTime += delta;
    if(anim.mElapsedTime > anim.mDurations.at(anim.mFrame - anim.mStart))
    {
        anim.mElapsedTime = 0;
        anim.mFrame++;
        
        if(anim.mFrame > anim.mEnd)
            anim.mFrame = anim.mStart;
    }
}

void AnimatedSprite::SetAnimation(int id)
{
    mActiveAnimationId = id;
}

void AnimatedSprite::DefineAnimation(int id, unsigned int start, unsigned int end, const FrameDurations& durations)
{
    mDefinedAnimations[id] = AnimationSequence(start, end, durations);
}




