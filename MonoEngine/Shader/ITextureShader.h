//
//  ITextureShader.h
//  MonoOSX
//
//  Created by Niklas Damberg on 26/12/14.
//
//

#pragma once

#include "IShader.h"

namespace mono
{
    struct Color;
    
    class ITextureShader : public IShader
    {
    public:

        virtual ~ITextureShader()
        { }

        virtual unsigned int GetPositionAttributeLocation() const = 0;
        virtual unsigned int GetTextureAttributeLocation() const = 0;
        
        virtual void SetShade(const mono::Color& color) = 0;
        virtual void SetAlphaTexture(bool isAlpha) = 0;
    };
}
