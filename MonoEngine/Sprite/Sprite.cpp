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
        
    static const unsigned short indices[] = { 0, 2, 1, 0, 3, 2 };
    
    void GenerateTextureCoordinates(int rows, int columns, std::vector<Math::Quad>& coordinates)
    {
        const float ystep = 1.0f / rows;
        const float xstep = 1.0f / columns;
        
        for(int yindex = rows; yindex > 0; --yindex)
        {
            for(int xindex = 0; xindex < columns; ++xindex)
            {
                const float x = xstep * xindex;
                const float y = ystep * (yindex -1);
                
                coordinates.push_back(Math::Quad(x, y, xstep, ystep));
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

    const lua::MapIntIntTable animations = lua::GetTableMap<int, int>(config, "animations");
    for(lua::MapIntIntTable::const_iterator it = animations.begin(), end = animations.end(); it != end; ++it)
    {
        const int key = it->first;
        const lua::IntTable& values = it->second;
        DefineAnimation(key, values);
    }
    
    std::vector<int> defaultDurations;
    defaultDurations.push_back(0);
    defaultDurations.push_back(-1);
    defaultDurations.push_back(0);
    defaultDurations.push_back(-1);
    
    //DefineAnimation(DEFAULT_ANIMATION_ID, defaultDurations);
    mActiveAnimationId = DEFAULT_ANIMATION_ID;
}

void Sprite::Draw() const
{
    const AnimationSequence& anim = mDefinedAnimations.find(mActiveAnimationId)->second;
    const Math::Quad& quad = mTextureCoordinates.at(anim.Frame());
    
    const float coords[] = { quad.mX,           quad.mY,
                             quad.mX,           quad.mY + quad.mH,
                             quad.mX + quad.mW, quad.mY + quad.mH,
                             quad.mX + quad.mW, quad.mY };
    
    mTexture->Use();
    
    glColor3f(1.0f, 1.0f, 1.0f);
    
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    
    glVertexPointer(2, GL_FLOAT, 0, vertices);
    glTexCoordPointer(2, GL_FLOAT, 0, coords);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, indices);

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
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

void Sprite::DefineAnimation(int id, const std::vector<int>& values)
{
    const bool even = (values.size() % 2) == 0;
    if(!even)
        throw std::runtime_error("Animation vector does not match up, not an even number of values");
    
    typedef std::vector<int> IntVector;
    IntVector::const_iterator lastFrame = values.end();
    --lastFrame;
    --lastFrame;
    
    const int start = *values.begin();
    const int end = *lastFrame;
    
    std::vector<unsigned int> durations;
    for(IntVector::const_iterator it = values.begin(), end = values.end(); it != end; ++it)
    {
        ++it;
        durations.push_back(*it);
    }
    
    const int diff = end - start;
    if(diff < 0)
        throw std::runtime_error("Animation definition's start and end does not match up");
                                 
    mDefinedAnimations[id] = AnimationSequence(start, end, durations);
}




