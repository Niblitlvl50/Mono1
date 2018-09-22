
#include "ShaderFactory.h"
#include "TextureShader.h"
#include "ColorShader.h"
#include "PointSpriteShader.h"

using namespace mono;

std::unique_ptr<mono::ITextureShader> ShaderFactory::CreateTextureShader() const
{
    return std::make_unique<mono::TextureShader>();
}

std::unique_ptr<mono::IColorShader> ShaderFactory::CreateColorShader() const
{
    return std::make_unique<mono::ColorShader>();
}

std::unique_ptr<mono::IPointSpriteShader> ShaderFactory::CreatePointSpriteShader() const
{
    return std::make_unique<mono::PointSpriteShader>();
}
