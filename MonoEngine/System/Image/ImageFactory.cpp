
#include "ImageFactory.h"
#include <stdexcept>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image/stb_image.h"

namespace
{
    struct Bitmap : public System::IImage
    {
        Bitmap(const byte* data, unsigned int width, unsigned int height, int colorComponents)
            : mData(data),
              mWidth(width),
              mHeight(height),
              mColorComponents(colorComponents)
        { }
        
        virtual const byte* Data() const
        {
            return mData;
        }
        virtual unsigned int Width() const
        {
            return mWidth;
        }
        virtual unsigned int Height() const
        {
            return mHeight;
        }
        virtual int ColorComponents() const
        {
            return mColorComponents;
        }

        const byte* mData;
        const unsigned int mWidth;
        const unsigned int mHeight;
        const int mColorComponents;
    };
}

System::IImagePtr System::LoadImage(const char* source)
{
    int width;
    int height;
    int components;
    const unsigned char* data = stbi_load(source, &width, &height, &components, 0);
    if(!data)
        throw std::runtime_error("Unable to load image!");

    return std::make_shared<Bitmap>(data, width, height, components);
}

System::IImagePtr System::CreateImage(const byte* data, int width, int height, int colorComponents)
{
    return std::make_shared<Bitmap>(data, width, height, colorComponents);
}
