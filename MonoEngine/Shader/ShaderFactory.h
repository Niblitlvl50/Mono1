//
//  ShaderFactory.h
//  MonoOSX
//
//  Created by Niklas Damberg on 26/12/14.
//
//

#pragma once

#include "IShaderFactory.h"

namespace mono
{
    class ShaderFactory : public IShaderFactory
    {
    public:

        virtual std::shared_ptr<ITextureShader> CreateTextureShader() const;
        virtual std::shared_ptr<IColorShader> CreateColorShader() const;
    };
}

