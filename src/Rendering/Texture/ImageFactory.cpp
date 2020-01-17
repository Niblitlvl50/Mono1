
#include "ImageFactory.h"
#include "System/System.h"

#include <stdexcept>
#include <cstdio>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image/stb_image.h"

namespace
{
    struct Bitmap : public mono::IImage
    {
        Bitmap(const byte* data, uint32_t width, uint32_t height, int color_components)
            : m_data(data),
              m_width(width),
              m_height(height),
              m_color_components(color_components)
        { }
        
        const byte* Data() const override
        {
            return m_data;
        }
        uint32_t Width() const override
        {
            return m_width;
        }
        uint32_t Height() const override
        {
            return m_height;
        }
        int ColorComponents() const override
        {
            return m_color_components;
        }

        const byte* m_data;
        const uint32_t m_width;
        const uint32_t m_height;
        const int m_color_components;
    };
}

mono::IImagePtr mono::LoadImage(const char* source)
{
    int width;
    int height;
    int components;
    const unsigned char* image_data = stbi_load(source, &width, &height, &components, 0);
    if(!image_data)
    {
        System::Log("ImageFactory|Unable to load '%s'\n", source);
        throw std::runtime_error("Unable to load image!");
    }

    return std::make_shared<Bitmap>(image_data, width, height, components);
}

mono::IImagePtr mono::LoadImageFromData(const unsigned char* data, int data_length)
{
    int width;
    int height;
    int components;
    const unsigned char* image_data = stbi_load_from_memory(data, data_length, &width, &height, &components, 0);
    if(!image_data)
    {
        System::Log("ImageFactory|Unable to load from data chunk\n");
        throw std::runtime_error("Unable to load image!");
    }

    return std::make_shared<Bitmap>(image_data, width, height, components);
}

mono::IImagePtr mono::CreateImage(const byte* data, int width, int height, int colorComponents)
{
    return std::make_shared<Bitmap>(data, width, height, colorComponents);
}
