
#include "ShaderFactory.h"
#include "IShader.h"
#include "TextureShader.h"
#include "ColorShader.h"
#include "PointSpriteShader.h"
#include "ImGuiShader.h"

using namespace mono;

std::unique_ptr<mono::IShader> ShaderFactory::CreateTextureShader() const
{
    return TextureShader::MakeShader();
}

std::unique_ptr<mono::IShader> ShaderFactory::CreateColorShader() const
{
    return ColorShader::MakeShader();
}

std::unique_ptr<mono::IShader> ShaderFactory::CreatePointSpriteShader() const
{
    return PointSpriteShader::MakeShader();
}

std::unique_ptr<mono::IShader> ShaderFactory::CreateImGuiShader() const
{
    return ImGuiShader::MakeShader();
}
