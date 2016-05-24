//
//  PNGImage.cpp
//  Monolith1
//
//  Created by Niblit on 2011-12-10.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#include "PNGImage.h"

#include "png.h"
#include "SysFile.h"
#include <stdexcept>


namespace
{
    constexpr unsigned int PNG_HEADER_SIZE = 8;

    // Same as GL_RGBA!
    constexpr unsigned int RGBA = 0x1908;
    
    struct libpngStruct
    {
        libpngStruct()
            : png(0),
              info(0)
        { }
        
        ~libpngStruct()
        {
            png_destroy_read_struct(&png, &info, 0);
        }
        
        png_structp png;
        png_infop info;
    };
}

using namespace libpng;

PNGImage::PNGImage(const std::string& source)
    : mSource(source)
{
    ReadSource();
}

void PNGImage::ReadSource()
{
    File::FilePtr file = File::OpenBinaryFile(mSource);
    
    unsigned char header[PNG_HEADER_SIZE];
    fread(header, 1, PNG_HEADER_SIZE, file.get());
    const int headerResult = png_sig_cmp(header, 0, 8);
    if(headerResult != 0)
        throw std::runtime_error("Failed to read png header");
    
    libpngStruct png;
    
    png.png = png_create_read_struct(PNG_LIBPNG_VER_STRING, 0, 0, 0);
    if(!png.png)
        throw std::runtime_error("Failed to create read struct");
    
    png.info = png_create_info_struct(png.png);
    if(!png.info)
        throw std::runtime_error("Failed to create info struct");
        
    const int jmpResult = setjmp(png_jmpbuf(png.png));
    if(jmpResult != 0)
        throw std::runtime_error("Failed to set jump(?)");
    
    png_init_io(png.png, file.get());
    png_set_sig_bytes(png.png, PNG_HEADER_SIZE);
    png_read_info(png.png, png.info);
    
    mWidth = png_get_image_width(png.png, png.info);
    mHeight = png_get_image_height(png.png, png.info);
    mColorType = png_get_color_type(png.png, png.info);
    mBitDepth = png_get_bit_depth(png.png, png.info);
    mChannels = png_get_channels(png.png, png.info);
    
    setjmp(png_jmpbuf(png.png));
   
    //Array of row pointers. One for every row.
    png_bytep* rowPtrs = new png_bytep[mHeight];
    
    //Alocate a buffer with enough space.
    unsigned char* data = new unsigned char[mWidth * mHeight * mBitDepth * mChannels / 8];
    
    //This is the length in bytes, of one row.
    const unsigned int stride = mWidth * mBitDepth * mChannels / 8;
    
    //A little for-loop here to set all the row pointers to the starting
    //Adresses for every row in the buffer
    for(unsigned int i = 0; i < mHeight; ++i)
    {
        //Set the pointer to the data pointer + i times the row stride.
        //Notice that the row order is reversed with q.
        //This is how at least OpenGL expects it,
        //and how many other image loaders present the data.
        unsigned int q = (mHeight - i - 1) * stride;
        rowPtrs[i] = (png_bytep)data + q;
    }

    png_read_image(png.png, rowPtrs);
    
    mData.reset(data);
    
    delete[] rowPtrs;
}

const byte* PNGImage::Data() const
{
    return mData.get();
}

unsigned int PNGImage::Width() const
{
    return mWidth;
}

unsigned int PNGImage::Height() const
{
    return mHeight;
}

int PNGImage::ColorComponents() const
{
    return RGBA;

    /*
    if(mColorType & PNG_COLOR_TYPE_RGBA)
        return 4;
    else if(mColorType & PNG_COLOR_TYPE_RGB)
        return 3;
    else if(mColorType & PNG_COLOR_TYPE_GRAY_ALPHA)
        return 2;
    else if(mColorType & PNG_COLOR_TYPE_GRAY)
        return 1;
    
    // This indicates error.
    return 0;
     */
}

unsigned int PNGImage::TargetFormat() const
{
    return RGBA;
}

bool PNGImage::HasAlpha() const
{
    return (mColorType == PNG_COLOR_MASK_ALPHA);
}

