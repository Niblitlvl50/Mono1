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
    
    static const float vertices[] = { -0.5f, -0.5f,
                                      -0.5f,  0.5f,
                                       0.5f,  0.5f,
                                       0.5f, -0.5f };
        
    static const unsigned short indices[] = { 0, 2, 1, 0, 3, 2 };
    
    void GenerateTextureCoordinates(int rows, int columns, const Math::Quad& coords, std::vector<Math::Quad>& coordinates)
    {
        const Math::Vector2f size = coords.mB - coords.mA;

        const float xstep = size.mX / columns;
        const float ystep = size.mY / rows;
        
        for(int yindex = rows; yindex > 0; --yindex)
        {
            for(int xindex = 0; xindex < columns; ++xindex)
            {
                const float x = xstep * xindex + coords.mA.mX;
                const float y = ystep * yindex + coords.mA.mY;
                
                coordinates.push_back(Math::Quad(x, y, x + xstep, y + ystep));
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
    
    Math::Quad coords;
    coords.mA.mX = lua::GetValue<float>(config, "x");
    coords.mA.mY = lua::GetValue<float>(config, "y");
    coords.mB.mX = lua::GetValue<float>(config, "u");
    coords.mB.mY = lua::GetValue<float>(config, "v");
    
    mTexture = mono::CreateTexture(texture);
    GenerateTextureCoordinates(rows, columns, coords, mTextureCoordinates);

    const lua::MapIntIntTable animations = lua::GetTableMap<int, int>(config, "animations");
    for(lua::MapIntIntTable::const_iterator it = animations.begin(), end = animations.end(); it != end; ++it)
    {
        const int key = it->first;
        const lua::IntTable& values = it->second;
        DefineAnimation(key, values);
    }
    
    mActiveAnimationId = DEFAULT_ANIMATION_ID;
}

void Sprite::Draw() const
{
    const AnimationSequence& anim = mDefinedAnimations.find(mActiveAnimationId)->second;
    const Math::Quad& quad = mTextureCoordinates.at(anim.Frame());
    
    const float coords[] = { quad.mA.mX, quad.mA.mY,
                             quad.mA.mX, quad.mB.mY,
                             quad.mB.mX, quad.mB.mY,
                             quad.mB.mX, quad.mA.mY };
    
    mTexture->Use();
    
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    
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




