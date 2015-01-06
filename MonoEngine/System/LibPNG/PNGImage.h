//
//  PNGImage.h
//  Monolith1
//
//  Created by Niblit on 2011-12-10.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#pragma once

#include "IImage.h"
#include <string>

namespace libpng
{
    struct PNGImage : mono::IImage
    {
        PNGImage(const std::string& source);
        
    private:
        
        void ReadSource();

        virtual const byte* Data() const;
        virtual unsigned int Width() const;
        virtual unsigned int Height() const;
        virtual int ColorComponents() const;
        virtual unsigned int TargetFormat() const;
        virtual bool HasAlpha() const;
        
        const std::string mSource;
        unsigned int mWidth;
        unsigned int mHeight;
        unsigned char mColorType;
        unsigned char mBitDepth;
        unsigned char mChannels;
        
        std::shared_ptr<byte> mData;
    };
}

