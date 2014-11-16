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

#include <stdexcept>
#include <iostream>
#include <map>

namespace
{
    typedef std::weak_ptr<mono::ITexture> ITextureWeakPtr; 
    std::map<std::string, ITextureWeakPtr> textureStorage;

    struct TextureDeleter
    {
        TextureDeleter(const std::string& source)
            : mSource(source)
        { }
    
        void operator()(mono::ITexture* tex) const
        {
            textureStorage.erase(mSource);
            delete tex;
        }
        
        const std::string mSource;
    };
}

mono::ITexturePtr mono::CreateTexture(const std::string& source)
{
    auto it = textureStorage.find(source);
    if(it != textureStorage.end())
    {
        mono::ITexturePtr texture = it->second.lock();
        if(texture)
            return texture;

        std::cout << "Error when trying to create a shared_ptr from weak_ptr using source: " << source << std::endl;
    }
    
    const mono::IImagePtr image = LoadImage(source);
    mono::ITexturePtr texture(new Texture(image), TextureDeleter(source));
    textureStorage[source] = texture;
    
    return texture;
}

mono::ITexturePtr mono::CreateTexture(const byte* data, int width, int height, int colorComponents, unsigned int targetFormat)
{
    const mono::IImagePtr image = CreateImage(data, width, height, colorComponents, targetFormat);
    return std::make_shared<Texture>(image);
}


