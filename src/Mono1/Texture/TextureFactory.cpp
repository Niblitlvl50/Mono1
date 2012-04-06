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
    typedef std::map<std::string, mono::ITexturePtr> TextureStoreMap;
    TextureStoreMap textureStore;
}

mono::ITexturePtr mono::CreateTexture(const std::string& source)
{
    TextureStoreMap::iterator it = textureStore.find(source);
    if(it != textureStore.end())
        return it->second;
    
    const mono::IImagePtr image = LoadImage(source);
    mono::ITexturePtr texture(new Texture(image));
    textureStore[source] = texture;
    
    return texture;
}


