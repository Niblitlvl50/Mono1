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
    
    
    struct TextureData
    {
        int rows;
        int columns;
        int x;
        int y;
        int u;
        int v;
        int textureSizeX;
        int textureSizeY;
    };
    
    void GenerateTextureCoordinates(const TextureData& data, std::vector<Math::Quad>& coordinates)
    {
        // +1 because the textures coordinates is zero indexed
        const float x1 = float(data.x +1) / float(data.textureSizeX);
        const float y1 = float(data.textureSizeY - data.v) / float(data.textureSizeY);
        const float x2 = float(data.u +1) / float(data.textureSizeX);
        const float y2 = float(data.textureSizeY - data.y) / float(data.textureSizeY);
        const float sizex = x2 - x1;
        const float sizey = y2 - y1;
        
        const float xstep = sizex / data.columns;
        const float ystep = sizey / data.rows;
        
        for(int yindex = data.rows; yindex > 0; --yindex)
        {
            for(int xindex = 0; xindex < data.columns; ++xindex)
            {
                const float x = xstep * xindex + x1;
                const float y = ystep * yindex + y1;
                
                coordinates.push_back(Math::Quad(x, y - ystep, x + xstep, y));
            }
        }
    }

}

Sprite::Sprite(const std::string& file)
{
    lua::LuaState config(file);

    const std::string texture = lua::GetValue<std::string>(config, "texture");
    
    TextureData data;
    data.rows = lua::GetValue<int>(config, "rows");
    data.columns = lua::GetValue<int>(config, "columns");
    data.x = lua::GetValue<int>(config, "x");
    data.y = lua::GetValue<int>(config, "y");
    data.u = lua::GetValue<int>(config, "u");
    data.v = lua::GetValue<int>(config, "v");
    
    mTexture = mono::CreateTexture(texture);
    
    data.textureSizeX = mTexture->Width();
    data.textureSizeY = mTexture->Height();
    
    GenerateTextureCoordinates(data, mTextureCoordinates);    

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




