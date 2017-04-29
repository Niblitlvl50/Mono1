
#include "TextureFactory.h"
#include "Texture.h"
#include "System/Image/IImage.h"
#include "System/Image/ImageFactory.h"

#include <cstdio>
#include <unordered_map>
#include <string>

namespace
{
    // This is where all the weak pointers goes, that points to the allocated textures!
    std::unordered_map<std::string, std::weak_ptr<mono::ITexture>> textureStorage;
}

mono::ITexturePtr mono::CreateTexture(const char* source)
{
    auto it = textureStorage.find(source);
    if(it != textureStorage.end())
    {
        mono::ITexturePtr texture = it->second.lock();
        if(texture)
            return texture;

        std::printf("TextureFactory - Unable to create a shared_ptr from weak_ptr using source: %s\n", source);
    }
    
    const System::IImagePtr image = System::LoadImage(source);

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

mono::ITexturePtr mono::CreateTexture(const unsigned char* data, int width, int height, int colorComponents)
{
    const System::IImagePtr image = System::CreateImage(data, width, height, colorComponents);
    return std::make_shared<Texture>(image);
}


