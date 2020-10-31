
#include "ShaderFactory.h"
#include "IShader.h"
#include "TextureShader.h"
#include "ColorShader.h"
#include "PointSpriteShader.h"
#include "ScreenShader.h"
#include "ImGuiShader.h"
#include "SpriteShader.h"

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

std::unique_ptr<mono::IShader> ShaderFactory::CreateScreenShader() const
{
    return ScreenShader::MakeShader();
}

std::unique_ptr<mono::IShader> ShaderFactory::CreateImGuiShader() const
{
    return ImGuiShader::MakeShader();
}

std::unique_ptr<mono::IShader> ShaderFactory::CreateSpriteShader() const
{
    return SpriteShader::MakeShader();
}
