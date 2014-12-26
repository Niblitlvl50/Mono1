//
//  ShaderFactory.cpp
//  MonoOSX
//
//  Created by Niklas Damberg on 26/12/14.
//
//

#include "ShaderFactory.h"
#include "TextureShader.h"
#include "ColorShader.h"

using namespace mono;

std::shared_ptr<mono::ITextureShader> ShaderFactory::CreateTextureShader() const
{
    return std::make_shared<mono::TextureShader>();
}

std::shared_ptr<mono::IColorShader> ShaderFactory::CreateColorShader() const
{
    return std::make_shared<mono::ColorShader>();
}

mono::IShaderFactory* shaderFactory = nullptr;