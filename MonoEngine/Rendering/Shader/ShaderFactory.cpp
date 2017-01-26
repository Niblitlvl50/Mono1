
#include "ShaderFactory.h"
#include "TextureShader.h"
#include "ColorShader.h"
#include "MorphingShader.h"
#include "PointSpriteShader.h"

using namespace mono;

std::shared_ptr<mono::ITextureShader> ShaderFactory::CreateTextureShader() const
{
    return std::make_shared<mono::TextureShader>();
}

std::shared_ptr<mono::IColorShader> ShaderFactory::CreateColorShader() const
{
    return std::make_shared<mono::ColorShader>();
}

std::shared_ptr<mono::IMorphingShader> ShaderFactory::CreateMorphingShader() const
{
    return std::make_shared<mono::MorphingShader>();
}

std::shared_ptr<mono::IPointSpriteShader> ShaderFactory::CreatePointSpriteShader() const
{
    return std::make_shared<mono::PointSpriteShader>();
}
