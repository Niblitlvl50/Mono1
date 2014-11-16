//
//  ImageFactory.cpp
//  Monolith1
//
//  Created by Niblit on 2011-12-10.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#include "ImageFactory.h"
#include <stdexcept>

#include "PNGImage.h"

namespace
{
    struct Bitmap : public mono::IImage
    {
        Bitmap(const byte* data, int width, int height, int colorComponents, unsigned int targetFormat)
            : mData(data),
              mWidth(width),
              mHeight(height),
              mColorComponents(colorComponents),
              mTargetFormat(targetFormat),
              mAlpha(false)
        { }
        
        virtual const byte* Data() const
        {
            return mData;
        }
        virtual int Width() const
        {
            return mWidth;
        }
        virtual int Height() const
        {
            return mHeight;
        }
        virtual int ColorComponents() const
        {
            return mColorComponents;
        }
        virtual unsigned int TargetFormat() const
        {
            return mTargetFormat;
        }
        virtual bool HasAlpha() const
        {
            return mAlpha;
        }

        const byte* mData;
        const int mWidth;
        const int mHeight;
        const int mColorComponents;
        const unsigned int mTargetFormat;
        const bool mAlpha;
    };
}


mono::IImagePtr mono::LoadImage(const std::string& source)
{
    const size_t dotpos = source.find_last_of(".");
    if(dotpos == std::string::npos)
        throw std::runtime_error("Unable to determine extension");
    
    const std::string extension = source.substr(dotpos +1);
    if(extension == "png")
        return std::make_shared<libpng::PNGImage>(source);

    // If we end up here, the image is unsupported.
    throw std::runtime_error("Unsupported image");
}

mono::IImagePtr mono::CreateImage(const byte* data, int width, int height, int colorComponents, unsigned int targetFormat)
{
    return std::make_shared<Bitmap>(data, width, height, colorComponents, targetFormat);
}
