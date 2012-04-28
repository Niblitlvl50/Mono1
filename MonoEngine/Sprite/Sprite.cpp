//
//  AnimatedSprite.cpp
//  Mono1
//
//  Created by Niblit on 2012-04-09.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "Sprite.h"
#include "ITexture.h"
#include "TextureFactory.h"
#include "SysOpenGL.h"
#include "LuaState.h"
#include "LuaFunctions.h"

#include <stdexcept>


using namespace mono;

namespace
{
    static const int DEFAULT_ANIMATION_ID = 0;
    
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

Sprite::Sprite(const std::string& file)
{
    lua::LuaState config(file);
    
    const std::string texture = lua::GetValue<std::string>(config, "texture");
    const int rows = lua::GetValue<int>(config, "rows");
    const int columns = lua::GetValue<int>(config, "columns");
    
    mTexture = mono::CreateTexture(texture);
    GenerateTextureCoordinates(rows, columns, mTextureCoordinates);

    const bool hasAnimations = lua::GetValue<bool>(config, "hasAnimations");
    if(hasAnimations)
    {
        // Do stuff here....
    }
    
    FrameDurations durations(1, -1);
    DefineAnimation(DEFAULT_ANIMATION_ID, 0, 0, durations);
    mActiveAnimationId = DEFAULT_ANIMATION_ID;
}

void Sprite::Draw() const
{
    const AnimationSequence& anim = mDefinedAnimations.find(mActiveAnimationId)->second;
    const SpriteTextureCoord& texcoords = mTextureCoordinates.at(anim.Frame());
    
    mTexture->Use();
    
    glColor3f(1.0f, 1.0f, 1.0f);
    
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    
    glTexCoordPointer(2, GL_FLOAT, 0, &texcoords);
    glVertexPointer(2, GL_FLOAT, 0, vertices);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, indices);

    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
}

void Sprite::doUpdate(unsigned int delta)
{
    AnimationSequence& anim = mDefinedAnimations[mActiveAnimationId];
    anim.Tick(delta);
}

void Sprite::SetAnimation(int id)
{
    mActiveAnimationId = id;
}

void Sprite::DefineAnimation(int id, unsigned int start, unsigned int end, const FrameDurations& durations)
{
    const int diff = end - start;
    if(diff < 0)
        throw std::runtime_error("Animation definition's start and end does not match up");
                                 
    mDefinedAnimations[id] = AnimationSequence(start, end, durations);
}




