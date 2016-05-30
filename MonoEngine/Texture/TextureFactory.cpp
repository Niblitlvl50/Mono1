//
//  TextureFactory.cpp
//  Monolith1
//
//  Created by Niblit on 2011-12-10.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#include "TextureFactory.h"
#include "Texture.h"
#include "IImage.h"
#include "ImageFactory.h"

#include <cstdio>
#include <unordered_map>

namespace
{
    // This is where all the weak pointers goes, that points to the allocated textures!
    std::unordered_map<std::string, std::weak_ptr<mono::ITexture>> textureStorage;
}

mono::ITexturePtr mono::CreateTexture(const std::string& source)
{
    auto it = textureStorage.find(source);
    if(it != textureStorage.end())
    {
        mono::ITexturePtr texture = it->second.lock();
        if(texture)
            return texture;

        std::printf("Error when trying to create a shared_ptr from weak_ptr using source: %s\n", source.c_str());
    }
    
    const mono::IImagePtr image = LoadImage(source);

    // Custom deleter that erases the texture from the map
    // when all the references are gone.
    const auto deleter = [source](mono::ITexture* ptr) {
        textureStorage.erase(source);
        delete ptr;
    };

    mono::ITexturePtr texture(new Texture(image), deleter);
    textureStorage[source] = texture;
    
    return texture;
}

mono::ITexturePtr mono::CreateTexture(const byte* data, int width, int height, int colorComponents, unsigned int targetFormat)
{
    const mono::IImagePtr image = CreateImage(data, width, height, colorComponents, targetFormat);
    return std::make_shared<Texture>(image);
}


