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
#include "LuaState.h"
#include "LuaFunctions.h"

#include <stdexcept>

using namespace mono;

namespace
{
    constexpr int DEFAULT_ANIMATION_ID = 0;

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
        coordinates.reserve(data.rows * data.columns);

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

                coordinates.emplace_back(x, y - ystep, x + xstep, y);
            }
        }
    }

}

Sprite::Sprite(const std::string& file)
{
    lua::LuaState config(file);

    const std::string& texture = lua::GetValue<std::string>(config, "texture");
    mTexture = mono::CreateTexture(texture);
    
    TextureData data;
    data.rows    = lua::GetValue<int>(config, "rows");
    data.columns = lua::GetValue<int>(config, "columns");
    data.x       = lua::GetValue<int>(config, "x");
    data.y       = lua::GetValue<int>(config, "y");
    data.u       = lua::GetValue<int>(config, "u");
    data.v       = lua::GetValue<int>(config, "v");
    
    data.textureSizeX = mTexture->Width();
    data.textureSizeY = mTexture->Height();
    
    GenerateTextureCoordinates(data, mTextureCoordinates);    

    const lua::MapIntIntTable animations = lua::GetTableMap<int, int>(config, "animations");
    const lua::MapIntStringTable attributes = lua::GetTableMap<int, std::string>(config, "attributes");
    
    for(const auto& animation : animations)
    {
        const int key = animation.first;
        const lua::IntTable& values = animation.second;
        
        auto attribute = attributes.find(key);
        if(attribute == attributes.end())
           DefineAnimation(key, values, true);
        else
           DefineAnimation(key, values, attribute->second);
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
    return anim.Finished() ? math::zeroQuad : mTextureCoordinates.at(anim.Frame());
}

const Color::RGBA& Sprite::GetShade() const
{
    return mColor;
}

void Sprite::SetShade(const mono::Color::RGBA& color)
{
    mColor = color;
}

void Sprite::doUpdate(unsigned int delta)
{
    AnimationSequence& anim = mDefinedAnimations.find(mActiveAnimationId)->second;
    anim.Tick(delta);

    if(anim.Finished() && mCallbackFunction)
        mCallbackFunction();
}

void Sprite::SetAnimation(int id)
{
    SetAnimation(id, nullptr);
}

void Sprite::SetAnimation(int id, const std::function<void ()>& func)
{
    mActiveAnimationId = id;
    mCallbackFunction = func;

    mDefinedAnimations.find(id)->second.Restart();
}

void Sprite::DefineAnimation(int id, const std::vector<int>& frames, bool loop)
{
    const bool even = (frames.size() % 2) == 0;
    if(!even)
        throw std::runtime_error("Animation vector does not match up, not an even number of values");

    AnimationSequence sequence(loop);

    for(auto it = frames.begin(), end = frames.end(); it != end; ++it)
    {
        const unsigned int frame = *it;
        ++it;
        const unsigned int duration = *it;

        sequence.AddFrame(frame, duration);
    }

    mDefinedAnimations.insert(std::make_pair(id, sequence));
}

void Sprite::DefineAnimation(int id, const std::vector<int>& frames, const std::vector<std::string>& attributes)
{
    const bool noloop = std::find_if(attributes.begin(), attributes.end(), FindNoLoopAttribute()) != attributes.end();
    DefineAnimation(id, frames, !noloop);
}

int Sprite::GetDefinedAnimations() const
{
    return static_cast<int>(mDefinedAnimations.size());
}

int Sprite::GetActiveAnimation() const
{
    return mActiveAnimationId;
}



