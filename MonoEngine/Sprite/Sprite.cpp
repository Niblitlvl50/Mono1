//
//  AnimatedSprite.cpp
//  Mono1
//
//  Created by Niblit on 2012-04-09.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "Sprite.h"
#include "SpriteAttributes.h"
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
    static const math::Quad nullQuad(0, 0, 0, 0);
        
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
    
    void GenerateTextureCoordinates(const TextureData& data, std::vector<math::Quad>& coordinates)
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
                const float x = xstep * float(xindex) + x1;
                const float y = ystep * float(yindex) + y1;
                
                coordinates.push_back(math::Quad(x, y - ystep, x + xstep, y));
            }
        }
    }

}

Sprite::Sprite(const std::string& file)
{
    lua::LuaState config(file);

    const std::string texture = lua::GetValue<std::string>(config, "texture");
    mTexture = mono::CreateTexture(texture);
    
    TextureData data;
    data.rows = lua::GetValue<int>(config, "rows");
    data.columns = lua::GetValue<int>(config, "columns");
    data.x = lua::GetValue<int>(config, "x");
    data.y = lua::GetValue<int>(config, "y");
    data.u = lua::GetValue<int>(config, "u");
    data.v = lua::GetValue<int>(config, "v");    
    
    data.textureSizeX = mTexture->Width();
    data.textureSizeY = mTexture->Height();
    
    GenerateTextureCoordinates(data, mTextureCoordinates);    

    const lua::MapIntIntTable animations = lua::GetTableMap<int, int>(config, "animations");
    const lua::MapIntStringTable attributes = lua::GetTableMap<int, std::string>(config, "attributes");
    
    for(lua::MapIntIntTable::const_iterator it = animations.begin(), end = animations.end(); it != end; ++it)
    {
        const int key = it->first;
        const lua::IntTable& values = it->second;
        
        lua::MapIntStringTable::const_iterator attr = attributes.find(key);
        if(attr == attributes.end())
           DefineAnimation(key, values, true);
        else
           DefineAnimation(key, values, attr->second);
    }
    
    mActiveAnimationId = DEFAULT_ANIMATION_ID;
}

ITexturePtr Sprite::GetTexture() const
{
    return mTexture;
}

const math::Quad& Sprite::GetTextureCoords() const
{
    const AnimationSequence& anim = mDefinedAnimations.find(mActiveAnimationId)->second;
    return anim.Finished() ? nullQuad : mTextureCoordinates.at(anim.Frame());
}

void Sprite::doUpdate(unsigned int delta)
{
    AnimationSequence& anim = mDefinedAnimations.find(mActiveAnimationId)->second;
    anim.Tick(delta);

    if(anim.Finished())
        mCallbackFunction();
}

void Sprite::SetAnimation(int id)
{
    mActiveAnimationId = id;
}

void Sprite::SetAnimation(int id, std::tr1::function<void ()> func)
{
    mActiveAnimationId = id;
    mCallbackFunction = func;
}

void Sprite::DefineAnimation(int id, const std::vector<int>& frames, bool loop)
{
    const bool even = (frames.size() % 2) == 0;
    if(!even)
        throw std::runtime_error("Animation vector does not match up, not an even number of values");
    
    std::vector<int>::const_iterator lastFrame = frames.end();
    --lastFrame;
    --lastFrame;
    
    const int start = *frames.begin();
    const int end = *lastFrame;
    
    std::vector<unsigned int> durations;
    for(std::vector<int>::const_iterator it = frames.begin(), end = frames.end(); it != end; ++it)
    {
        ++it;
        durations.push_back(*it);
    }
    
    const int diff = end - start;
    if(diff < 0)
        throw std::runtime_error("Animation definition's start and end does not match up");
    
    mDefinedAnimations.insert(std::make_pair(id, AnimationSequence(start, end, loop, durations)));
}

void Sprite::DefineAnimation(int id, const std::vector<int>& frames, const std::vector<std::string>& attributes)
{
    const bool noloop = std::find_if(attributes.begin(), attributes.end(), FindNoLoopAttribute()) != attributes.end();
    DefineAnimation(id, frames, !noloop);
}




