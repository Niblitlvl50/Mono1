
#pragma once

#include "IShader.h"

namespace mono
{
    namespace Color
    {
        struct RGBA;
    }
    
    class ITextureShader : public IShader
    {
    public:

        virtual ~ITextureShader()
        { }

        virtual unsigned int GetPositionAttributeLocation() const = 0;
        virtual unsigned int GetTextureAttributeLocation() const = 0;
        
        virtual void SetShade(const mono::Color::RGBA& color) = 0;
        virtual void SetAlphaTexture(bool isAlpha) = 0;
        virtual void SetTextureOffset(float offset) = 0;
    };
}
